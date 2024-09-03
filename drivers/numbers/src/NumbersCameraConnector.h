#pragma once
#include "CameraConnector.h"

namespace VCA::EXAMPLES
{
    class NumbersCameraConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit NumbersCameraConnector() = default;
        ~NumbersCameraConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::EXAMPLES

#ifndef TESTING
extern "C" VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::EXAMPLES::NumbersCameraConnector();
}
#endif