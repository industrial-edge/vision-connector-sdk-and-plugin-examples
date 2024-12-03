#pragma once
#include <Camera.h>
#include <vector>

namespace VCA::Globbing
{
    class GlobbingCamera : public VCA::SDK::v1::Camera
    {
    public:
        explicit GlobbingCamera() = delete;
        explicit GlobbingCamera(const std::string& url);
        ~GlobbingCamera() override = default;

        std::shared_ptr<VCA::SDK::v1::Image> acquireImage() override;

        SDK::v1::CameraParameters getConfig() override;
        SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) override;
        SDK::v1::CameraStatus getStatus() const override;

        const std::vector<std::string>& imagePaths() const;

    protected:
        void startImageAcquisition() override;
        void stopImageAcquisition() override;

    private:
        int m_fps = 1;

        std::vector<std::string> m_imagePaths;
        size_t m_index = 0;

        void collectImages();
    };
} // namespace VCA::Globbing