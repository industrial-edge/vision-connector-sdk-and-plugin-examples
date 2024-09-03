#pragma once
#include "CameraConnector.h"
#include <VmbCPP/VmbCPP.h>

namespace VCA::allied
{
    class alliedConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit alliedConnector() = default;
        ~alliedConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::allied

#ifndef TESTING
extern "C" VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::allied::alliedConnector();
}
#endif