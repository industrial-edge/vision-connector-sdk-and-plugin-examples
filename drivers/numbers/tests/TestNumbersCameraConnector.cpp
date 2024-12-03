#include "../src/NumbersCameraConnector.h"

#include <gtest/gtest.h>

TEST(TestNumbersCameraConnector, testDiscover)
{
    VCA::Numbers::NumbersCameraConnector numbersCameraConnector;
    const auto discoveredCameras = numbersCameraConnector.discover();

    EXPECT_EQ(discoveredCameras.size(), 4);
    EXPECT_EQ(discoveredCameras.at(0), "red");
    EXPECT_EQ(discoveredCameras.at(1), "green");
    EXPECT_EQ(discoveredCameras.at(2), "blue");
    EXPECT_EQ(discoveredCameras.at(3), "black");
}

TEST(TestNumbersCameraConnector, testCreateCameras)
{
    VCA::Numbers::NumbersCameraConnector numbersCameraConnector;

    const auto redCamera = numbersCameraConnector.createCamera("red");
    const auto greenCamera = numbersCameraConnector.createCamera("green");
    const auto blueCamera = numbersCameraConnector.createCamera("blue");
    const auto blackCamera = numbersCameraConnector.createCamera("black");

    EXPECT_EQ(redCamera->cameraUniqueId(), "red");
    EXPECT_EQ(greenCamera->cameraUniqueId(), "green");
    EXPECT_EQ(blueCamera->cameraUniqueId(), "blue");
    EXPECT_EQ(blackCamera->cameraUniqueId(), "black");
}