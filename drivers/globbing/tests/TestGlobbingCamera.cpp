#include "../src/GlobbingCamera.h"
#include "CameraException.h"
#include "CameraParameter.h"
#include "CameraParameterStatus.h"

#include <chrono>
#include <cstring>
#include <filesystem>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <opencv2/imgcodecs.hpp>

using njson = nlohmann::json;
namespace fs = std::filesystem;

constexpr const char* GLOBBING_DATA_FOLDER = "./globbing_data/";
constexpr const char* CUSTOM_GLOBBING_DATA_FOLDER = "./custom_globbing_data/";

class TestGlobbingCamera : public ::testing::Test
{
protected:
    void TearDown() override
    {
        fs::remove_all(GLOBBING_DATA_FOLDER);
        fs::remove_all(CUSTOM_GLOBBING_DATA_FOLDER);
    }
};

TEST_F(TestGlobbingCamera, testCameraStatus)
{
    auto globbingCamera = VCA::EXAMPLES::GlobbingCamera("");
    EXPECT_EQ(globbingCamera.getStatus().status(), VCA::SDK::v1::CameraStatus::Status::OFFLINE);
    globbingCamera.connect();
    EXPECT_EQ(globbingCamera.getStatus().status(), VCA::SDK::v1::CameraStatus::Status::ONLINE);
    globbingCamera.disconnect();
    EXPECT_EQ(globbingCamera.getStatus().status(), VCA::SDK::v1::CameraStatus::Status::OFFLINE);
}

TEST_F(TestGlobbingCamera, testAcquireImageWithoutImageSourceFolder)
{
    auto globbingCamera = VCA::EXAMPLES::GlobbingCamera(std::string(GLOBBING_DATA_FOLDER) + "birds");
    globbingCamera.connect();

    try
    {
        globbingCamera.acquireImage();
        FAIL();
    }
    catch (const VCA::SDK::v1::CameraException& ce)
    {
        ASSERT_STREQ("Globbing camera error: No image to read", ce.what());
    }
    catch (...)
    {
        FAIL();
    }
}

TEST_F(TestGlobbingCamera, testAcquireImageWithRemovedImage)
{
    const auto birdsImageFolder = std::string(GLOBBING_DATA_FOLDER) + "birds";
    fs::create_directories(birdsImageFolder);
    fs::copy("./resources/birds", birdsImageFolder);

    auto globbingCamera = VCA::EXAMPLES::GlobbingCamera(birdsImageFolder);
    globbingCamera.connect();

    fs::remove_all(birdsImageFolder);

    try
    {
        globbingCamera.acquireImage();
        FAIL();
    }
    catch (const VCA::SDK::v1::CameraException& ce)
    {
        ASSERT_STREQ("Globbing camera error: Unable to read image file: ./globbing_data/birds/bird_1.jpg", ce.what());
    }
    catch (...)
    {
        FAIL();
    }
}

TEST_F(TestGlobbingCamera, testAcquireImage)
{
    const auto birdsImageFolder = std::string(GLOBBING_DATA_FOLDER) + "birds";
    fs::create_directories(birdsImageFolder);
    fs::copy("./resources/birds", birdsImageFolder);

    auto globbingCamera = VCA::EXAMPLES::GlobbingCamera(birdsImageFolder);
    globbingCamera.connect();

    for (int i = 0; i < 5; ++i)
    {
        const auto image = globbingCamera.acquireImage();
        const auto customFieldsJson = njson::parse(image->customFields());
        const auto imagePath = customFieldsJson["image_path"].get<std::string>();
        const auto imageMat = cv::imread(imagePath);

        EXPECT_EQ(std::memcmp(imageMat.data, image->buffer(0), image->bufferSize(0)), 0);
    }
}

TEST_F(TestGlobbingCamera, testAcquireImageWithCustomImageFolder)
{
    const auto birdsImageFolder = std::string(CUSTOM_GLOBBING_DATA_FOLDER) + "birds";
    fs::create_directories(birdsImageFolder);
    fs::copy("./resources/birds", birdsImageFolder);

    auto globbingCamera = VCA::EXAMPLES::GlobbingCamera(birdsImageFolder);
    globbingCamera.connect();

    for (int i = 0; i < 5; ++i)
    {
        const auto image = globbingCamera.acquireImage();
        const auto customFieldsJson = njson::parse(image->customFields());
        const auto imagePath = customFieldsJson["image_path"].get<std::string>();
        const auto imageMat = cv::imread(imagePath);

        EXPECT_EQ(std::memcmp(imageMat.data, image->buffer(0), image->bufferSize(0)), 0);
    }
}

TEST_F(TestGlobbingCamera, testAcquireImageWithChangingImageFolder)
{
    const auto birdsImageFolder = std::string(GLOBBING_DATA_FOLDER) + "birds";
    fs::create_directories(birdsImageFolder);
    fs::copy("./resources/birds", birdsImageFolder);

    auto globbingCamera = VCA::EXAMPLES::GlobbingCamera(birdsImageFolder);
    globbingCamera.connect();

    EXPECT_EQ(globbingCamera.imagePaths().size(), 5);

    for (size_t i = 0; i < globbingCamera.imagePaths().size() - 1; ++i)
    {
        const auto image = globbingCamera.acquireImage();
        const auto customFieldsJson = njson::parse(image->customFields());
        const auto imagePath = customFieldsJson["image_path"].get<std::string>();
        const auto imageMat = cv::imread(imagePath);

        EXPECT_EQ(std::memcmp(imageMat.data, image->buffer(0), image->bufferSize(0)), 0);
    }

    fs::remove(birdsImageFolder + "/bird_1.jpg");
    fs::remove(birdsImageFolder + "/bird_2.jpg");

    globbingCamera.acquireImage();

    EXPECT_EQ(globbingCamera.imagePaths().size(), 3);
}

TEST_F(TestGlobbingCamera, testAcquireImageWithIncreasedFPS)
{
    const auto birdsImageFolder = std::string(GLOBBING_DATA_FOLDER) + "birds";
    fs::create_directories(birdsImageFolder);
    fs::copy("./resources/birds", birdsImageFolder);

    auto globbingCamera = VCA::EXAMPLES::GlobbingCamera(birdsImageFolder);
    globbingCamera.connect();

    const auto getConfigResult1 = globbingCamera.getConfig();
    EXPECT_EQ(getConfigResult1.size(), 1);
    EXPECT_EQ(getConfigResult1.at(0).name(), "FPS");
    EXPECT_EQ(getConfigResult1.at(0).value(), "1");
    EXPECT_EQ(getConfigResult1.at(0).type(), "int");
    EXPECT_EQ(getConfigResult1.at(0).description(), "FPS");
    EXPECT_EQ(getConfigResult1.at(0).min(), "1");
    EXPECT_EQ(getConfigResult1.at(0).max(), "1000");
    EXPECT_EQ(getConfigResult1.at(0).readOnly(), false);

    VCA::SDK::v1::CameraParameter fpsParameter;
    fpsParameter.setName("FPS");
    fpsParameter.setValue("1000");

    const auto setConfigResult = globbingCamera.setConfig(VCA::SDK::v1::CameraParameters{fpsParameter});
    EXPECT_EQ(setConfigResult.size(), 1);
    EXPECT_EQ(setConfigResult.at(0).name(), "FPS");
    EXPECT_EQ(setConfigResult.at(0).value(), "1000");
    EXPECT_EQ(setConfigResult.at(0).status(), VCA::SDK::v1::CameraParameterStatus::Status::OK);

    const auto getConfigResult2 = globbingCamera.getConfig();
    EXPECT_EQ(getConfigResult2.size(), 1);
    EXPECT_EQ(getConfigResult2.at(0).name(), "FPS");
    EXPECT_EQ(getConfigResult2.at(0).value(), "1000");
    EXPECT_EQ(getConfigResult2.at(0).type(), "int");
    EXPECT_EQ(getConfigResult2.at(0).description(), "FPS");
    EXPECT_EQ(getConfigResult2.at(0).min(), "1");
    EXPECT_EQ(getConfigResult2.at(0).max(), "1000");
    EXPECT_EQ(getConfigResult2.at(0).readOnly(), false);

    const auto startTime = std::chrono::system_clock::now();

    for (int i = 0; i < 10; ++i)
    {
        const auto image = globbingCamera.acquireImage();
        const auto customFieldsJson = njson::parse(image->customFields());
        const auto imagePath = customFieldsJson["image_path"].get<std::string>();
        const auto imageMat = cv::imread(imagePath);

        EXPECT_EQ(std::memcmp(imageMat.data, image->buffer(0), image->bufferSize(0)), 0);
    }

    const auto endTime = std::chrono::system_clock::now();
    const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    EXPECT_TRUE(elapsedTime.count() < 1000);
}