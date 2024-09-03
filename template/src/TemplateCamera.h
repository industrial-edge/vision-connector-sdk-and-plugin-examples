#pragma once
#include "Camera.h"

namespace VCA::Template
{
    class TemplateCamera : public VCA::SDK::v1::Camera
    {
    public:
        explicit TemplateCamera() = delete;
        explicit TemplateCamera(const std::string& cameraType);
        ~TemplateCamera() override = default;

        std::shared_ptr<VCA::SDK::v1::Image> acquireImage() override;

        SDK::v1::CameraParameters getConfig() override;
        SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) override;
        SDK::v1::CameraStatus getStatus() const override;

    protected:
        void startImageAcquisition() override;
        void stopImageAcquisition() override;

    private:
        //private Variables
    };
} // namespace VCA::Template