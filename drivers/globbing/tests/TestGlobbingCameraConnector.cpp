#include "../src/GlobbingCameraConnector.h"

#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;

constexpr const char* GLOBBING_DATA_FOLDER = "/globbing_data/";

class TestCameraConnector : public ::testing::Test
{
protected:
    void TearDown() override
    {
        if (fs::exists(GLOBBING_DATA_FOLDER))
        {
            fs::remove_all(GLOBBING_DATA_FOLDER);
        }
    }
};

TEST_F(TestCameraConnector, testDiscoverWithoutFolder)
{
    VCA::Globbing::GlobbingCameraConnector globbingCameraConnector;
    const auto discoveredCameras = globbingCameraConnector.discover();

    ASSERT_TRUE(discoveredCameras.empty());
}

TEST_F(TestCameraConnector, testDiscoverWithEmptyFolder)
{
    fs::create_directories(GLOBBING_DATA_FOLDER);

    VCA::Globbing::GlobbingCameraConnector globbingCameraConnector;
    const auto discoveredCameras = globbingCameraConnector.discover();

    ASSERT_TRUE(discoveredCameras.empty());
}

TEST_F(TestCameraConnector, testDiscover)
{
    fs::create_directories(GLOBBING_DATA_FOLDER);
    fs::create_directories(std::string(GLOBBING_DATA_FOLDER) + "data1");
    fs::create_directories(std::string(GLOBBING_DATA_FOLDER) + "data2");

    VCA::Globbing::GlobbingCameraConnector globbingCameraConnector;
    const auto discoveredCameras = globbingCameraConnector.discover();

    ASSERT_EQ(discoveredCameras.size(), 2);
    ASSERT_EQ(discoveredCameras.at(0), std::string(GLOBBING_DATA_FOLDER) + "data1");
    ASSERT_EQ(discoveredCameras.at(1), std::string(GLOBBING_DATA_FOLDER) + "data2");
}

TEST_F(TestCameraConnector, testCreateCameraWithoutFolder)
{

    VCA::Globbing::GlobbingCameraConnector globbingCameraConnector;
    const auto globbingCamera = globbingCameraConnector.createCamera("globbing_data_2/data1");
    ASSERT_EQ(globbingCamera->cameraUniqueId(), "globbing_data_2/data1");
}

TEST_F(TestCameraConnector, testCreateCameraWithFolder)
{

    VCA::Globbing::GlobbingCameraConnector globbingCameraConnector;
    const auto globbingCamera = globbingCameraConnector.createCamera(std::string(GLOBBING_DATA_FOLDER) + "data1");
    ASSERT_EQ(globbingCamera->cameraUniqueId(), std::string(GLOBBING_DATA_FOLDER) + "data1");
}