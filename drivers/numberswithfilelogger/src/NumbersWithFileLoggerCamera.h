#pragma once
#include <Camera.h>
#include <opencv2/opencv.hpp>

namespace VCA::NumbersWithFileLogger
{
    class NumbersWithFileLoggerCamera : public VCA::SDK::v1::Camera
    {
    public:
        explicit NumbersWithFileLoggerCamera() = delete;
        explicit NumbersWithFileLoggerCamera(const std::string& cameraType);
        ~NumbersWithFileLoggerCamera() override = default;

        std::shared_ptr<VCA::SDK::v1::Image> acquireImage() override;

        SDK::v1::CameraParameters getConfig() override;
        SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) override;
        SDK::v1::CameraStatus getStatus() const override;

    protected:
        void startImageAcquisition() override;
        void stopImageAcquisition() override;

    private:
        int m_width = 1920;
        int m_height = 1080;
        cv::Scalar m_backgroundColor;

        double m_fontScale = 1.0;
        int m_thickness = 2;
        int m_baseline = 0;
        int m_fps = 1;
    };
} // namespace VCA::NumbersWithFileLogger