#include "../src/baslerllCamera.h"
#include "../src/baslerllConnector.h"

#include <gtest/gtest.h>

using namespace VCA::baslerll;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

TEST(TestBaslerllCameraConnector, testDiscover)
{
    baslerllConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    std::cout  << "Checking found cameras" << std::endl;
    for(int i =0; i < discoveredCameras.size(); i++){
        std::cout  << discoveredCameras[i] << std::endl;
    }
    EXPECT_TRUE(discoveredCameras.size()>0) << "Looking for";
    
}

TEST(TestBaslerllCamera, testGetConfig)
{
    baslerllConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    if(discoveredCameras.size() > 0){
        auto camera = baslerllCamera(discoveredCameras[0]);
        auto config = camera.safeGetConfig();
        EXPECT_TRUE(config.size()>0) << "Configurations";
        config[2].setValue("On");
        camera.safeSetConfig(config);
        config = camera.safeGetConfig();
    }
}


TEST(TestBaslerllCamera, testAcquireImage)
{
    baslerllConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    if(discoveredCameras.size() > 0){
        auto camera = baslerllCamera(discoveredCameras[0]);
        camera.connect();
        const auto image1 = camera.safeAcquireImage();
        EXPECT_TRUE(image1->bufferSize(0) > 0);
        const auto image2 = camera.safeAcquireImage();
        EXPECT_TRUE(image2->bufferSize(0) > 0);
        camera.disconnect();
    }
}