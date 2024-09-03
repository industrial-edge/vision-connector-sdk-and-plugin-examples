#include "../src/baslerCamera.h"
#include "../src/baslerConnector.h"

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

TEST(TestBaslerCameraConnector, testDiscover)
{
    VCA::basler::baslerConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    std::cout  << "Checking found cameras" << std::endl;
    for(int i =0; i < discoveredCameras.size(); i++){
        std::cout  << discoveredCameras[i] << std::endl;
    }
    EXPECT_TRUE(discoveredCameras.size()>0) << "Looking for";
    
}


TEST(TestBaslerCamera, testAcquireImage)
{
    VCA::basler::baslerConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    if(discoveredCameras.size() > 0){
        auto camera = VCA::basler::baslerCamera(discoveredCameras[0]);
        camera.connect();
        const auto image1 = camera.acquireImage();
        EXPECT_TRUE(image1->bufferSize(0) > 0);
        const auto image2 = camera.acquireImage();
        EXPECT_TRUE(image2->bufferSize(0) > 0);
        camera.disconnect();
    }
}
