#pragma once
#include <thread>
#include "Camera.h"
#include <pylon/PylonIncludes.h>
using namespace Pylon;

namespace VCA::basler
{
    class baslerCamera : public VCA::SDK::v1::Camera
    {
    public:
        explicit baslerCamera() = delete;
        explicit baslerCamera(const std::string& cameraType);
        ~baslerCamera() override = default;

        std::shared_ptr<VCA::SDK::v1::Image> acquireImage() override;

        SDK::v1::CameraParameters getConfig() override;
        SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) override;
        SDK::v1::CameraStatus getStatus() const override;

    protected:
        void startImageAcquisition() override;
        void stopImageAcquisition() override;

    private:
        //private Variables
        std::string pixelTypeToString(EPixelType pixelType);
        std::unique_ptr<CInstantCamera> m_camera;
        void Open();
        void Close();
    };
} // namespace VCA::basler