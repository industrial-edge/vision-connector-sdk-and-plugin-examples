#pragma once
#include "CameraConnector.h"
#include <pylon/PylonIncludes.h>

namespace VCA::basler
{
    class baslerConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit baslerConnector() = default;
        ~baslerConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::basler

#ifndef TESTING
extern "C" VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::basler::baslerConnector();
}
#endif
