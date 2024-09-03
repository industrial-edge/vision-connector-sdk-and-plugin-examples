#pragma once
#include "Camera.h"
#include <VmbCPP/VmbCPP.h>

using namespace VmbCPP;

namespace VCA::allied
{
    class alliedCamera : public VCA::SDK::v1::Camera
    {
    public:
        explicit alliedCamera() = delete;
        explicit alliedCamera(const std::string& cameraType);
        ~alliedCamera() override = default;

        std::shared_ptr<VCA::SDK::v1::Image> acquireImage() override;

        SDK::v1::CameraParameters getConfig() override;
        SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) override;
        SDK::v1::CameraStatus getStatus() const override;

    protected:
        void startImageAcquisition() override;
        void stopImageAcquisition() override;

    private:
        std::string pixelTypeToString(VmbPixelFormatType pixelFormat);
        CameraPtr  m_camera;
        VmbSystem *m_vmbSystem;
        //private Variables
    };
} // namespace VCA::allied