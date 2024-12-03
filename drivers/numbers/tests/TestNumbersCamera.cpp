#include "../src/NumbersCamera.h"

#include <gtest/gtest.h>

cv::Mat createImageBackground(int width, int height, cv::Scalar color)
{
    cv::Mat image(height, width, CV_8UC3, color);
    return image;
}

cv::Mat putTextOnImage(const cv::Mat& image, const std::string& text, double fontScale, int thickness, int baseline)
{
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);

    int x = (image.cols - textSize.width) / 2;
    int y = (image.rows + textSize.height) / 2;

    cv::putText(image, text, cv::Point(x, y), fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);

    return image;
}

TEST(TestNumbersCamera, testCameraStatus)
{
    auto numbersCamera = VCA::Numbers::NumbersCamera("");
    EXPECT_EQ(numbersCamera.getStatus().status(), VCA::SDK::v1::CameraStatus::Status::OFFLINE);
    numbersCamera.connect();
    EXPECT_EQ(numbersCamera.getStatus().status(), VCA::SDK::v1::CameraStatus::Status::ONLINE);
    numbersCamera.disconnect();
    EXPECT_EQ(numbersCamera.getStatus().status(), VCA::SDK::v1::CameraStatus::Status::OFFLINE);
}

TEST(TestNumbersCamera, testAcquireRedImage)
{
    auto numbersCamera = VCA::Numbers::NumbersCamera("red");
    numbersCamera.connect();

    auto image1 = numbersCamera.acquireImage();
    auto referenceImage1 = createImageBackground(1920, 1080, cv::Scalar(0, 0, 255));
    putTextOnImage(referenceImage1, "1", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage1.data, image1->buffer(0), image1->bufferSize(0)), 0);

    numbersCamera.disconnect();
    numbersCamera.connect();

    auto image2 = numbersCamera.acquireImage();
    auto referenceImage2 = createImageBackground(1920, 1080, cv::Scalar(0, 0, 255));
    putTextOnImage(referenceImage2, "2", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage2.data, image2->buffer(0), image2->bufferSize(0)), 0);
}

TEST(TestNumbersCamera, testAcquireGreenImage)
{
    auto numbersCamera = VCA::Numbers::NumbersCamera("green");
    numbersCamera.connect();

    auto image1 = numbersCamera.acquireImage();
    auto referenceImage1 = createImageBackground(1920, 1080, cv::Scalar(0, 255, 0));
    putTextOnImage(referenceImage1, "1", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage1.data, image1->buffer(0), image1->bufferSize(0)), 0);

    numbersCamera.disconnect();
    numbersCamera.connect();

    auto image2 = numbersCamera.acquireImage();
    auto referenceImage2 = createImageBackground(1920, 1080, cv::Scalar(0, 255, 0));
    putTextOnImage(referenceImage2, "2", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage2.data, image2->buffer(0), image2->bufferSize(0)), 0);
}

TEST(TestNumbersCamera, testAcquireBlueImage)
{
    auto numbersCamera = VCA::Numbers::NumbersCamera("blue");
    numbersCamera.connect();

    auto image1 = numbersCamera.acquireImage();
    auto referenceImage1 = createImageBackground(1920, 1080, cv::Scalar(255, 0, 0));
    putTextOnImage(referenceImage1, "1", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage1.data, image1->buffer(0), image1->bufferSize(0)), 0);

    numbersCamera.disconnect();
    numbersCamera.connect();

    auto image2 = numbersCamera.acquireImage();
    auto referenceImage2 = createImageBackground(1920, 1080, cv::Scalar(255, 0, 0));
    putTextOnImage(referenceImage2, "2", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage2.data, image2->buffer(0), image2->bufferSize(0)), 0);
}

TEST(TestNumbersCamera, testAcquireBlackImage)
{
    auto numbersCamera = VCA::Numbers::NumbersCamera("");
    numbersCamera.connect();

    auto image1 = numbersCamera.acquireImage();
    auto referenceImage1 = createImageBackground(1920, 1080, cv::Scalar(0, 0, 0));
    putTextOnImage(referenceImage1, "1", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage1.data, image1->buffer(0), image1->bufferSize(0)), 0);

    numbersCamera.disconnect();
    numbersCamera.connect();

    auto image2 = numbersCamera.acquireImage();
    auto referenceImage2 = createImageBackground(1920, 1080, cv::Scalar(0, 0, 0));
    putTextOnImage(referenceImage2, "2", 1.0, 2, 0);
    EXPECT_EQ(std::memcmp(referenceImage2.data, image2->buffer(0), image2->bufferSize(0)), 0);
}

TEST(TestNumbersCamera, testAcquireModifiedImage)
{
    auto numbersCamera = VCA::Numbers::NumbersCamera("");
    numbersCamera.connect();

    VCA::SDK::v1::CameraParameter widthParameter;
    widthParameter.setName("Width");
    widthParameter.setValue("100");
    VCA::SDK::v1::CameraParameter heightParameter;
    heightParameter.setName("Height");
    heightParameter.setValue("100");
    VCA::SDK::v1::CameraParameter fontScaleParameter;
    fontScaleParameter.setName("Font Scale");
    fontScaleParameter.setValue("0.5");
    VCA::SDK::v1::CameraParameter thinknessParameter;
    thinknessParameter.setName("Thickness");
    thinknessParameter.setValue("1");
    VCA::SDK::v1::CameraParameter fpsParameter;
    fpsParameter.setName("FPS");
    fpsParameter.setValue("1000");

    const auto setConfigResult = numbersCamera.setConfig(
        {widthParameter, heightParameter, fontScaleParameter, thinknessParameter, fpsParameter});
    EXPECT_EQ(setConfigResult.size(), 5);
    EXPECT_EQ(setConfigResult.at(0).name(), "Width");
    EXPECT_EQ(setConfigResult.at(0).value(), "100");
    EXPECT_EQ(setConfigResult.at(0).status(), VCA::SDK::v1::CameraParameterStatus::Status::OK);
    EXPECT_EQ(setConfigResult.at(1).name(), "Height");
    EXPECT_EQ(setConfigResult.at(1).value(), "100");
    EXPECT_EQ(setConfigResult.at(1).status(), VCA::SDK::v1::CameraParameterStatus::Status::OK);
    EXPECT_EQ(setConfigResult.at(2).name(), "Font Scale");
    EXPECT_EQ(setConfigResult.at(2).value(), "0.5");
    EXPECT_EQ(setConfigResult.at(2).status(), VCA::SDK::v1::CameraParameterStatus::Status::OK);
    EXPECT_EQ(setConfigResult.at(3).name(), "Thickness");
    EXPECT_EQ(setConfigResult.at(3).value(), "1");
    EXPECT_EQ(setConfigResult.at(3).status(), VCA::SDK::v1::CameraParameterStatus::Status::OK);
    EXPECT_EQ(setConfigResult.at(4).name(), "FPS");
    EXPECT_EQ(setConfigResult.at(4).value(), "1000");
    EXPECT_EQ(setConfigResult.at(4).status(), VCA::SDK::v1::CameraParameterStatus::Status::OK);

    const auto getConfigResult = numbersCamera.getConfig();
    EXPECT_EQ(getConfigResult.size(), 5);
    EXPECT_EQ(getConfigResult.at(0).name(), "Width");
    EXPECT_EQ(getConfigResult.at(0).value(), "100");
    EXPECT_EQ(getConfigResult.at(1).name(), "Height");
    EXPECT_EQ(getConfigResult.at(1).value(), "100");
    EXPECT_EQ(getConfigResult.at(2).name(), "Font Scale");
    EXPECT_EQ(getConfigResult.at(2).value(), "0.500000");
    EXPECT_EQ(getConfigResult.at(3).name(), "Thickness");
    EXPECT_EQ(getConfigResult.at(3).value(), "1");
    EXPECT_EQ(getConfigResult.at(4).name(), "FPS");
    EXPECT_EQ(getConfigResult.at(4).value(), "1000");

    auto image1 = numbersCamera.acquireImage();
    auto referenceImage1 = createImageBackground(100, 100, cv::Scalar(0, 0, 0));
    putTextOnImage(referenceImage1, "1", 0.5, 1, 0);
    EXPECT_EQ(std::memcmp(referenceImage1.data, image1->buffer(0), image1->bufferSize(0)), 0);

    numbersCamera.disconnect();
    numbersCamera.connect();

    auto image2 = numbersCamera.acquireImage();
    auto referenceImage2 = createImageBackground(100, 100, cv::Scalar(0, 0, 0));
    putTextOnImage(referenceImage2, "2", 0.5, 1, 0);
    EXPECT_EQ(std::memcmp(referenceImage2.data, image2->buffer(0), image2->bufferSize(0)), 0);
}

TEST(TestNumbersCamera, testAcquireImageWithIncreasedFPS)
{
    auto numbersCamera = VCA::Numbers::NumbersCamera("");
    numbersCamera.connect();

    VCA::SDK::v1::CameraParameter fpsParameter;
    fpsParameter.setName("FPS");
    fpsParameter.setValue("1000");

    const auto setConfigResult = numbersCamera.setConfig(VCA::SDK::v1::CameraParameters{fpsParameter});
    EXPECT_EQ(setConfigResult.size(), 1);
    EXPECT_EQ(setConfigResult.at(0).name(), "FPS");
    EXPECT_EQ(setConfigResult.at(0).value(), "1000");
    EXPECT_EQ(setConfigResult.at(0).status(), VCA::SDK::v1::CameraParameterStatus::Status::OK);

    const auto startTime = std::chrono::system_clock::now();

    for (int i = 0; i < 10; ++i)
    {
        numbersCamera.acquireImage();
    }

    const auto endTime = std::chrono::system_clock::now();
    const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    EXPECT_TRUE(elapsedTime.count() < 1000);
}