#pragma once
#include "CameraConnector.h"

namespace VCA::EXAMPLES
{
    class GlobbingCameraConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit GlobbingCameraConnector() = default;
        ~GlobbingCameraConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::EXAMPLES

#ifndef TESTING
extern "C" VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::EXAMPLES::GlobbingCameraConnector();
}
#endif