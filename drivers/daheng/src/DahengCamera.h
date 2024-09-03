#pragma once
#include "Camera.h"
#include "GxIAPI.h"

#include <iostream>
#include <thread>

namespace VCA::Daheng
{
    class DahengCamera : public VCA::SDK::v1::Camera
    {
    public:
        explicit DahengCamera() = delete;
        explicit DahengCamera(const std::string& cameraId);
        ~DahengCamera() override = default;

        std::shared_ptr<VCA::SDK::v1::Image> acquireImage() override;

        SDK::v1::CameraParameters getConfig() override;
        SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) override;
        SDK::v1::CameraStatus getStatus() const override;

    protected:
        void startImageAcquisition() override;
        void stopImageAcquisition() override;

    private:
        // private Variables
        std::string GetErrorString(GX_STATUS emErrorStatus);
        int64_t m_width;
        int64_t m_height;
        size_t m_bufferSize;
        std::string m_pixelFormat;
        const char* gx_pixel_format_to_string(GX_PIXEL_FORMAT_ENTRY pixel_format);
        GX_DEV_HANDLE m_hDevice = NULL;
        void GX_STDC OnFrameCallback(GX_FRAME_CALLBACK_PARAM* pFrame);
        void openCamera();
        void closeCamera();
        SDK::v1::CameraParameterStatus::Status SetCameraFPS(double fps);
        GX_FRAME_DATA framebuffer;

        struct CameraConfigList
        {
            std::string fps = "FPS";
            std::string TriggerSource = "Trigger Source";
            std::string AcquisitionMode = "Acquisition Mode";
            std::string TriggerMode = "Trigger Mode";
            std::string TriggerSelector = "Trigger Selector";
            std::string LineSelector = "Line Selector";
            std::string PixelFormat = "Pixelformat";
            std::string Width = "Width";
            std::string Height = "Height";

            std::string OutputFormat = "outputformat";
            std::string Cropping = "cropping";
        };
        CameraConfigList cameraConfig;
        SDK::v1::CameraParameter getConfigfps();
        SDK::v1::CameraParameter getTriggerSource();
        std::map<uint32_t, std::string> triggerSource = {{0, "Software trigger"}, {1, "Trigger source 0"},
                                                         {2, "Trigger source 1"}, {3, "Trigger source 2"},
                                                         {4, "Trigger source 3"}, {5, "Counter 2 end trigger"},
                                                         {6, "Trigger signal"},   {7, "MultiSource"},
                                                         {8, "CXPTrigger0"},      {9, "CXPTrigger1"}};

        SDK::v1::CameraParameter getAcquisitionMode();
        std::map<uint32_t, std::string> acquisitionMode = {{0, "Single frame"}, {1, "Multi frame"}, {2, "Continuous"}};

        SDK::v1::CameraParameter getTriggerMode();
        std::map<uint32_t, std::string> triggerMode = {{0, "Off"}, {1, "On"}};

        SDK::v1::CameraParameter getTriggerSelector();
        std::map<uint32_t, std::string> triggerSelector = {{1, "Frame Start"}, {2, "Frame Burst Start"}};

        SDK::v1::CameraParameter getLineSelector();
        std::map<uint32_t, std::string> lineSelector = {{0, "Line 0"},
                                                        {1, "Line 1"},
                                                        {2, "Line 2"},
                                                        {3, "Line 3"},
                                                        {4, "Line 4"},
                                                        {5, "Line 5"},
                                                        {6, "Line 6"},
                                                        {7, "Line 7"},
                                                        {8, "Line 8"},
                                                        {9, "Line 9"},
                                                        {10, "Line 10"},
                                                        {11, "Line strobe"},
                                                        {12, "Line 11"},
                                                        {13, "Line 12"},
                                                        {14, "Line 13"},
                                                        {15, "Line 14"},
                                                        {16, "Hardware trigger input"},
                                                        {17, "GPIO input"},
                                                        {18, "GPIO input"},
                                                        {19, "Flash_ B transmission"},
                                                        {20, "Flash_ B transmission"}};

        SDK::v1::CameraParameter getPixelFormat();
        std::map<int64_t, std::string> pixelFormat;
    };
} // namespace VCA::Daheng