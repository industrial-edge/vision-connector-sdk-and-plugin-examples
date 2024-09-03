#include "../src/DahengCamera.h"
#include "../src/DahengConnector.h"

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

TEST(TestDahengCameraConnector, testDiscover)
{
    VCA::Daheng::DahengConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    std::cout << "Checking found cameras" << std::endl;
    for (size_t i = 0; i < discoveredCameras.size(); ++i)
    {
        std::cout << discoveredCameras[i] << std::endl;
    }
    EXPECT_TRUE(discoveredCameras.size() == 0) << "Daheng camera emulation is not supported";
}

TEST(TestDahengCamera, testAcquireImage)
{
    VCA::Daheng::DahengConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    if (discoveredCameras.size() > 0)
    {
        auto camera = VCA::Daheng::DahengCamera(discoveredCameras[0]);
        camera.connect();
        using namespace std::chrono_literals;
        int q = 0;
        for (uint16_t i = 0; i < 5; ++i)
        {
            std::this_thread::sleep_for(1500ms);
            const auto image1 = camera.acquireImage();
            image1->count() > 0 ? q++ : q;
        }
        EXPECT_TRUE(q == 10);
        camera.disconnect();
    }
}

TEST(TestDahengCamera, testGetConfig)
{
    VCA::Daheng::DahengConnector cameraConnector;
    const auto discoveredCameras = cameraConnector.discover();
    if (discoveredCameras.size() > 0)
    {
        auto camera = VCA::Daheng::DahengCamera(discoveredCameras[0]);
        const auto params = camera.getConfig();
        EXPECT_TRUE(params.size() > 0);
        camera.connect();
        auto parameterStatuses = camera.setConfig(params);
        EXPECT_TRUE(parameterStatuses[0].status() == VCA::SDK::v1::CameraParameterStatus::Status::OK);
        camera.disconnect();
    }
}
