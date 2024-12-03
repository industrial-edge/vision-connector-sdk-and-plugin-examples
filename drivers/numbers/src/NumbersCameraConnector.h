#pragma once
#include "CameraConnector.h"

namespace VCA::Numbers
{
    class NumbersCameraConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit NumbersCameraConnector();
        ~NumbersCameraConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::Numbers
