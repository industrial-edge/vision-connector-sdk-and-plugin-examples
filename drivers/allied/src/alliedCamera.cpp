#include "CameraException.h"

#include "alliedCamera.h"

namespace VCA::allied
{
    alliedCamera::alliedCamera(const std::string& cameraType) : Camera(cameraType)
    {
        
    }

    std::string alliedCamera::pixelTypeToString(VmbPixelFormatType pixelFormat)
    {
        switch (pixelFormat)
        {
            // mono formats
            case VmbPixelFormatMono8:
                return "Mono8";
            case VmbPixelFormatMono10:
                return "Mono10";
            case VmbPixelFormatMono10p:
                return "Mono10p";
            case VmbPixelFormatMono12:
                return "Mono12";
            case VmbPixelFormatMono12Packed:
                return "Mono12Packed";
            case VmbPixelFormatMono12p:
                return "Mono12p";
            case VmbPixelFormatMono14:
                return "Mono14";
            case VmbPixelFormatMono16:
                return "Mono16";

                // bayer formats
            case VmbPixelFormatBayerGR8:
                return "BayerGR8";
            case VmbPixelFormatBayerRG8:
                return "BayerRG8";
            case VmbPixelFormatBayerGB8:
                return "BayerGB8";
            case VmbPixelFormatBayerBG8:
                return "BayerBG8";
            case VmbPixelFormatBayerGR10:
                return "BayerGR10";
            case VmbPixelFormatBayerRG10:
                return "BayerRG10";
            case VmbPixelFormatBayerGB10:
                return "BayerGB10";
            case VmbPixelFormatBayerBG10:
                return "BayerBG10";
            case VmbPixelFormatBayerGR12:
                return "BayerGR12";
            case VmbPixelFormatBayerRG12:
                return "BayerRG12";
            case VmbPixelFormatBayerGB12:
                return "BayerGB12";
            case VmbPixelFormatBayerBG12:
                return "BayerBG12";
            case VmbPixelFormatBayerGR12Packed:
                return "BayerGR12Packed";
            case VmbPixelFormatBayerRG12Packed:
                return "BayerRG12Packed";
            case VmbPixelFormatBayerGB12Packed:
                return "BayerGB12Packed";
            case VmbPixelFormatBayerBG12Packed:
                return "BayerBG12Packed";
            case VmbPixelFormatBayerGR10p:
                return "BayerGR10p";
            case VmbPixelFormatBayerRG10p:
                return "BayerRG10p";
            case VmbPixelFormatBayerGB10p:
                return "BayerGB10p";
            case VmbPixelFormatBayerBG10p:
                return "BayerBG10p";
            case VmbPixelFormatBayerGR12p:
                return "BayerGR12p";
            case VmbPixelFormatBayerRG12p:
                return "BayerRG12p";
            case VmbPixelFormatBayerGB12p:
                return "BayerGB12p";
            case VmbPixelFormatBayerBG12p:
                return "BayerBG12p";
            case VmbPixelFormatBayerGR16:
                return "BayerGR16";
            case VmbPixelFormatBayerRG16:
                return "BayerRG16";
            case VmbPixelFormatBayerGB16:
                return "BayerGB16";
            case VmbPixelFormatBayerBG16:
                return "BayerBG16";

                // rgb formats
            case VmbPixelFormatRgb8:
                return "RGB8";
            case VmbPixelFormatBgr8:
                return "BGR8";
            case VmbPixelFormatRgb10:
                return "RGB10";
            case VmbPixelFormatBgr10:
                return "BGR10";
            case VmbPixelFormatRgb12:
                return "RGB12";
            case VmbPixelFormatBgr12:
                return "BGR12";
            case VmbPixelFormatRgb14:
                return "RGB14";
            case VmbPixelFormatRgb16:
                return "RGB16";

                // other formats
            case VmbPixelFormatYuv411:
                return "YUV411";
            case VmbPixelFormatYuv422:
                return "YUV422";
            case VmbPixelFormatYuv444:
                return "YUV444";

                // default case
            default:
                return "Unknown";
        }
    }

    std::shared_ptr<VCA::SDK::v1::Image> alliedCamera::acquireImage()
    {
        FramePtr frame;
        VmbUint32_t width, height, imageSize;
        VmbUchar_t* imageBuffer;
        VmbPixelFormatType pixelFormat;
        VmbErrorType err;
        auto image = std::make_shared<SDK::v1::Image>();

        err = m_camera->AcquireSingleImage(frame, 5000);
        if (err != VmbErrorSuccess && err != VmbErrorTimeout)
        {
            throw VCA::SDK::v1::CameraException("Image collection error..." );
        }
        else if(err != VmbErrorTimeout)
        {
            frame->GetWidth(width);
            frame->GetHeight(height);
            frame->GetBufferSize(imageSize);
            frame->GetBuffer(imageBuffer);
            frame->GetPixelFormat(pixelFormat);

            const VCA::SDK::v1::ImageDetail imageDetail(
            VCA::SDK::v1::CameraInformation(cameraUniqueId(), "0"),
            VCA::SDK::v1::ImageInformation(imageSequenceCounter(), height, width, pixelTypeToString(pixelFormat)),
            VCA::SDK::v1::PTPInformation("Disabled", "0"));
            auto imageData = std::make_unique<SDK::v1::ImageData>((unsigned char *)imageBuffer, imageSize);
            image->addImagePart(std::move(imageData), imageDetail);
        }
        //Return image
        return image;
    }

    void alliedCamera::startImageAcquisition() {
        VmbErrorType err;

        m_vmbSystem = &VmbSystem::GetInstance();
        err = m_vmbSystem->Startup();

        if (err != VmbErrorSuccess)
        {
            throw VCA::SDK::v1::CameraException("Could not start API, err=" + std::to_string(err));
        }

        err = m_vmbSystem->OpenCameraByID(m_uniqueId.c_str(), VmbAccessModeFull, m_camera);
        if (err != VmbErrorSuccess)
        {
            throw VCA::SDK::v1::CameraException("Could not open camera, err=" + std::to_string(err));
        }

    }

    void alliedCamera::stopImageAcquisition() {
        m_vmbSystem->Shutdown();
    }

    SDK::v1::CameraParameters alliedCamera::getConfig()
    {
        SDK::v1::CameraParameters cameraParameters;
        //Push paramaters
        return cameraParameters;
    }

    SDK::v1::CameraParameterStatuses alliedCamera::setConfig(const SDK::v1::CameraParameters& parametersToChange)
    {
        SDK::v1::CameraParameterStatuses parameterStatuses;
        //Set parameters
        return parameterStatuses;
    }

    SDK::v1::CameraStatus alliedCamera::getStatus() const
    {
        const auto status =
            isConnected() ? SDK::v1::CameraStatus::Status::ONLINE : SDK::v1::CameraStatus::Status::OFFLINE;
        return SDK::v1::CameraStatus(status);
    }

} // namespace VCA::allied