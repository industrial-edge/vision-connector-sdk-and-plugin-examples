#pragma once
#include <CameraConnector.h>

namespace VCA::Globbing
{
    class GlobbingCameraConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit GlobbingCameraConnector();
        ~GlobbingCameraConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::Globbing