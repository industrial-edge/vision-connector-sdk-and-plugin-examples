#include "DahengCamera.h"

using namespace VCA::SDK::v1;
using namespace std;

#define GX_ErrorTrap(status)                                                                                           \
    if (status != GX_STATUS_SUCCESS)                                                                                   \
    {                                                                                                                  \
        auto errorString = GetErrorString(status);                                                                     \
        throw VCA::SDK::v1::CameraException(errorString);                                                              \
    }

namespace VCA::Daheng
{
    DahengCamera::DahengCamera(const std::string& cameraId) : Camera(cameraId)
    {
        m_version = "0.0.2";
    }

    std::string DahengCamera::GetErrorString(GX_STATUS emErrorStatus)
    {
        char* error_info = NULL;
        size_t size = 0;
        GX_STATUS emStatus = GX_STATUS_SUCCESS;

        // Get length of error description
        emStatus = GXGetLastError(&emErrorStatus, NULL, &size);
        if (emStatus != GX_STATUS_SUCCESS)
        {
            throw VCA::SDK::v1::CameraException("Error when calling GXGetLastError");
        }

        error_info = new char[size];
        emStatus = GXGetLastError(&emErrorStatus, error_info, &size);
        if (emStatus != GX_STATUS_SUCCESS)
        {
            throw VCA::SDK::v1::CameraException("Error when calling GXGetLastError");
        }

        std::string error(error_info);

        // Release error library
        if (error_info != NULL)
        {
            delete[] error_info;
        }
        return error;
    }

    const char* DahengCamera::gx_pixel_format_to_string(GX_PIXEL_FORMAT_ENTRY pixel_format)
    {
        switch (pixel_format)
        {
        case GX_PIXEL_FORMAT_UNDEFINED:
            return "Undefined";
        case GX_PIXEL_FORMAT_MONO8:
            return "Mono8";
        case GX_PIXEL_FORMAT_MONO8_SIGNED:
            return "Mono8Signed";
        case GX_PIXEL_FORMAT_MONO10:
            return "Mono10";
        case GX_PIXEL_FORMAT_MONO12:
            return "Mono12";
        case GX_PIXEL_FORMAT_MONO14:
            return "Mono14";
        case GX_PIXEL_FORMAT_MONO16:
            return "Mono16";
        case GX_PIXEL_FORMAT_BAYER_GR8:
            return "BayerGR8";
        case GX_PIXEL_FORMAT_BAYER_RG8:
            return "BayerRG8";
        case GX_PIXEL_FORMAT_BAYER_GB8:
            return "BayerGB8";
        case GX_PIXEL_FORMAT_BAYER_BG8:
            return "BayerBG8";
        case GX_PIXEL_FORMAT_BAYER_GR10:
            return "BayerGR10";
        case GX_PIXEL_FORMAT_BAYER_RG10:
            return "BayerRG10";
        case GX_PIXEL_FORMAT_BAYER_GB10:
            return "BayerGB10";
        case GX_PIXEL_FORMAT_BAYER_BG10:
            return "BayerBG10";
        case GX_PIXEL_FORMAT_BAYER_GR12:
            return "BayerGR12";
        case GX_PIXEL_FORMAT_BAYER_RG12:
            return "BayerRG12";
        case GX_PIXEL_FORMAT_BAYER_GB12:
            return "BayerGB12";
        case GX_PIXEL_FORMAT_BAYER_BG12:
            return "BayerBG12";
        case GX_PIXEL_FORMAT_BAYER_GR16:
            return "BayerGR16";
        case GX_PIXEL_FORMAT_BAYER_RG16:
            return "BayerRG16";
        case GX_PIXEL_FORMAT_BAYER_GB16:
            return "BayerGB16";
        case GX_PIXEL_FORMAT_BAYER_BG16:
            return "BayerBG16";
        case GX_PIXEL_FORMAT_RGB8_PLANAR:
            return "RGB8Planar";
        case GX_PIXEL_FORMAT_RGB10_PLANAR:
            return "RGB10Planar";
        case GX_PIXEL_FORMAT_RGB12_PLANAR:
            return "RGB12Planar";
        case GX_PIXEL_FORMAT_RGB16_PLANAR:
            return "RGB16Planar";
        case GX_PIXEL_FORMAT_RGB8:
            return "RGB8";
        case GX_PIXEL_FORMAT_RGB10:
            return "RGB10";
        case GX_PIXEL_FORMAT_RGB12:
            return "RGB12";
        case GX_PIXEL_FORMAT_RGB14:
            return "RGB14";
        case GX_PIXEL_FORMAT_RGB16:
            return "RGB16";
        case GX_PIXEL_FORMAT_BGR8:
            return "BGR8";
        case GX_PIXEL_FORMAT_BGR10:
            return "BGR10";
        case GX_PIXEL_FORMAT_BGR12:
            return "BGR12";
        case GX_PIXEL_FORMAT_BGR14:
            return "BGR14";
        case GX_PIXEL_FORMAT_BGR16:
            return "BGR16";
        default:
            return "Unknown";
        }
    }

    std::shared_ptr<VCA::SDK::v1::Image> DahengCamera::acquireImage()
    {
        auto image = std::make_shared<SDK::v1::Image>();
        if (m_hDevice && m_isConnected)
        {
            auto emStatus = GXGetImage(m_hDevice, &framebuffer, 2000);
            if (emStatus != GX_STATUS_SUCCESS)
            {
                if (emStatus != GX_STATUS_TIMEOUT)
                {
                    auto errorString = GetErrorString(emStatus);
                    throw VCA::SDK::v1::CameraException("Error grabbing frame." + errorString);
                }
            }
            else
            {
                auto format = std::string(gx_pixel_format_to_string((GX_PIXEL_FORMAT_ENTRY)framebuffer.nPixelFormat));
                const VCA::SDK::v1::ImageDetail imageDetail(
                    VCA::SDK::v1::CameraInformation(cameraUniqueId(), "0"),
                    VCA::SDK::v1::ImageInformation(increaseImageSequenceCounter(), framebuffer.nWidth,
                                                   framebuffer.nHeight, format),
                    VCA::SDK::v1::PTPInformation("Disabled", "0"));
                image->addImagePart((uint8_t*)framebuffer.pImgBuf, m_bufferSize, imageDetail);
            }
        }

        // Return image
        return image;
    }

    void DahengCamera::openCamera()
    {
        GX_OPEN_PARAM open_param;
        open_param.accessMode = GX_ACCESS_EXCLUSIVE;
        open_param.openMode = GX_OPEN_SN;
        open_param.pszContent = (char*)m_uniqueId.c_str();
        auto status = GXOpenDevice(&open_param, &m_hDevice);
        GX_ErrorTrap(status);
    }

    void DahengCamera::closeCamera()
    {
        auto emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
        GX_ErrorTrap(emStatus);
        // Close device
        emStatus = GXCloseDevice(m_hDevice);
        if (emStatus != GX_STATUS_SUCCESS)
        {
            auto errorString = GetErrorString(emStatus);
        }
        m_hDevice = NULL;
    }

    void DahengCamera::startImageAcquisition()
    {
        openCamera();
        // Device start acquisition
        auto status = GXStreamOn(m_hDevice);
        GX_ErrorTrap(status);

        // Allocate buffer
        if (framebuffer.pImgBuf != NULL)
        {
            delete[] (char*)framebuffer.pImgBuf;
            framebuffer.pImgBuf = NULL;
        }
        int64_t payloadSize;
        status = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &payloadSize);
        m_bufferSize = payloadSize;
        GX_ErrorTrap(status);
        framebuffer.pImgBuf = new char[m_bufferSize];
        if (framebuffer.pImgBuf == NULL)
        {
            printf("<Failed to allocate memory>\n");
            return;
        }

        status = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
        GX_ErrorTrap(status);
    }

    void DahengCamera::stopImageAcquisition()
    {
        closeCamera();
        // Release the resources
        if (framebuffer.pImgBuf != NULL)
        {
            delete[] (char*)framebuffer.pImgBuf;
            framebuffer.pImgBuf = NULL;
        }
    }

    SDK::v1::CameraParameter DahengCamera::getConfigfps()
    {
        SDK::v1::CameraParameter fpsCameraParameter;
        fpsCameraParameter.setName(cameraConfig.fps);
        fpsCameraParameter.setType("double");
        fpsCameraParameter.setDescription("FPS");

        double val;
        GXGetFloat(m_hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, &val);
        fpsCameraParameter.setValue(std::to_string(val));

        GX_FLOAT_RANGE range;
        GXGetFloatRange(m_hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, &range);

        fpsCameraParameter.setMin(std::to_string(range.dMin));
        fpsCameraParameter.setMax(std::to_string(range.dMax));
        fpsCameraParameter.setReadOnly(false);
        return fpsCameraParameter;
    }

    SDK::v1::CameraParameter DahengCamera::getTriggerSource()
    {
        SDK::v1::CameraParameter tsCamParam;
        tsCamParam.setName(cameraConfig.TriggerSource);
        tsCamParam.setType(VCA::SDK::v1::ParameterType::Enum);
        tsCamParam.setDescription("Trigger Source");
        int64_t current;
        GXGetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, &current);
        SDK::v1::Symbolics s;
        for (auto const& ts : triggerSource)
        {
            s.push_back(ts.second);
        }
        tsCamParam.setSymbolics(s);
        tsCamParam.setValue(triggerSource[current]);
        return tsCamParam;
    }

    SDK::v1::CameraParameter DahengCamera::getAcquisitionMode()
    {
        SDK::v1::CameraParameter tsCamParam;
        tsCamParam.setName(cameraConfig.AcquisitionMode);
        tsCamParam.setType(VCA::SDK::v1::ParameterType::Enum);
        tsCamParam.setDescription("Acquisition Mode");
        int64_t current;
        GXGetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, &current);
        SDK::v1::Symbolics s;
        for (auto const& ts : acquisitionMode)
        {
            s.push_back(ts.second);
        }
        tsCamParam.setSymbolics(s);
        tsCamParam.setValue(acquisitionMode[current]);
        return tsCamParam;
    }

    SDK::v1::CameraParameter DahengCamera::getTriggerMode()
    {
        SDK::v1::CameraParameter tsCamParam;
        tsCamParam.setName(cameraConfig.TriggerMode);
        tsCamParam.setType(VCA::SDK::v1::ParameterType::Enum);
        tsCamParam.setDescription("Trigger Mode");
        int64_t current;
        GXGetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, &current);
        SDK::v1::Symbolics s;
        for (auto const& ts : triggerMode)
        {
            s.push_back(ts.second);
        }
        tsCamParam.setSymbolics(s);
        tsCamParam.setValue(triggerMode[current]);
        return tsCamParam;
    }

    SDK::v1::CameraParameter DahengCamera::getTriggerSelector()
    {
        SDK::v1::CameraParameter tsCamParam;
        tsCamParam.setName(cameraConfig.TriggerSelector);
        tsCamParam.setType(VCA::SDK::v1::ParameterType::Enum);
        tsCamParam.setDescription("Trigger Selector");
        int64_t current;
        GXGetEnum(m_hDevice, GX_ENUM_TRIGGER_SELECTOR, &current);
        SDK::v1::Symbolics s;
        for (auto const& ts : triggerSelector)
            s.push_back(ts.second);
        tsCamParam.setSymbolics(s);
        tsCamParam.setValue(triggerSelector[current]);
        return tsCamParam;
    }

    SDK::v1::CameraParameter DahengCamera::getLineSelector()
    {
        SDK::v1::CameraParameter tsCamParam;
        tsCamParam.setName(cameraConfig.LineSelector);
        tsCamParam.setType(VCA::SDK::v1::ParameterType::Enum);
        tsCamParam.setDescription("Line Selector");
        int64_t current;
        GXGetEnum(m_hDevice, GX_ENUM_LINE_MODE, &current);
        SDK::v1::Symbolics s;
        for (auto const& ts : lineSelector)
        {
            s.push_back(ts.second);
        }
        tsCamParam.setSymbolics(s);
        tsCamParam.setValue(lineSelector[current]);
        return tsCamParam;
    }

    SDK::v1::CameraParameter DahengCamera::getPixelFormat()
    {
        SDK::v1::CameraParameter tsCamParam;
        tsCamParam.setName(cameraConfig.PixelFormat);
        tsCamParam.setType(VCA::SDK::v1::ParameterType::Enum);
        tsCamParam.setDescription("PixelFormat");

        uint32_t ui32EntryNums = 0;
        GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &ui32EntryNums);
        auto pEnumDescription = new GX_ENUM_DESCRIPTION[ui32EntryNums];
        auto nSize = ui32EntryNums * sizeof(GX_ENUM_DESCRIPTION);
        GXGetEnumDescription(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription, &nSize);
        int64_t i64CurrentValue = 0;
        GXGetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, &i64CurrentValue);
        SDK::v1::Symbolics s;
        pixelFormat.clear();
        for (uint32_t i = 0; i < ui32EntryNums; ++i)
        {
            pixelFormat.insert(
                std::pair<int64_t, std::string>(pEnumDescription[i].nValue, pEnumDescription[i].szSymbolic));
            s.push_back(std::string(pEnumDescription[i].szSymbolic));
        }

        int64_t current;
        GXGetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, &current);
        tsCamParam.setSymbolics(s);
        tsCamParam.setValue(pixelFormat[current]);
        return tsCamParam;
    }

    SDK::v1::CameraParameters DahengCamera::getConfig()
    {
        if (!isConnected())
        {
            openCamera();
        }
        SDK::v1::CameraParameters cameraParameters;
        cameraParameters.push_back(getConfigfps());
        cameraParameters.push_back(getTriggerSource());
        cameraParameters.push_back(getAcquisitionMode());
        cameraParameters.push_back(getTriggerMode());
        cameraParameters.push_back(getTriggerSelector());
        cameraParameters.push_back(getLineSelector());
        cameraParameters.push_back(getPixelFormat());

        int64_t width, height;
        GXGetInt(m_hDevice, GX_INT_WIDTH, &width);
        GXGetInt(m_hDevice, GX_INT_HEIGHT, &height);
        GX_INT_RANGE widthRange, heightRange;
        GXGetIntRange(m_hDevice, GX_INT_WIDTH, &widthRange);
        GXGetIntRange(m_hDevice, GX_INT_HEIGHT, &heightRange);
        cameraParameters.push_back(CameraParameter::CreateParameter(
            ParameterType::Int, cameraConfig.Width, to_string(width), "Sensor width", to_string(widthRange.nMin),
            to_string(widthRange.nMax), false, to_string(widthRange.nInc)));
        cameraParameters.push_back(CameraParameter::CreateParameter(
            ParameterType::Int, cameraConfig.Height, to_string(height), "Sensor height", to_string(heightRange.nMin),
            to_string(heightRange.nMax), false, to_string(heightRange.nInc)));

        if (!isConnected())
        {
            closeCamera();
        }
        return cameraParameters;
    }

    SDK::v1::CameraParameterStatus::Status DahengCamera::SetCameraFPS(double fps)
    {
        // Set to continuous acquisition
        auto status = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
        if (status != GX_STATUS_SUCCESS)
        {
            auto errorString = GetErrorString(status);
            return SDK::v1::CameraParameterStatus::Status::ERROR;
        }

        // Set the triggerMode off
        status = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_FRAME_RATE_MODE, GX_ACQUISITION_FRAME_RATE_MODE_ON);
        if (status != GX_STATUS_SUCCESS)
        {
            auto errorString = GetErrorString(status);
            return SDK::v1::CameraParameterStatus::Status::ERROR;
        }
        // Set the triggerMode off
        status = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
        if (status != GX_STATUS_SUCCESS)
        {
            auto errorString = GetErrorString(status);
            return SDK::v1::CameraParameterStatus::Status::ERROR;
        }

        status = GXSetFloat(m_hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, fps);
        if (status != GX_STATUS_SUCCESS)
        {
            auto errorString = GetErrorString(status);
            return SDK::v1::CameraParameterStatus::Status::ERROR;
        }
        return SDK::v1::CameraParameterStatus::Status::OK;
    }

    template <typename t, typename k>
    t findSecond(std::map<t, k> map, std::string item)
    {
        for (const auto& [key, value] : map)
        {
            if (value == item)
                return key;
        }
        return -1;
    }

    SDK::v1::CameraParameterStatuses DahengCamera::setConfig(const SDK::v1::CameraParameters& parametersToChange)
    {
        if (!isConnected())
        {
            openCamera();
        }
        SDK::v1::CameraParameterStatuses parameterStatuses;
        GX_STATUS status;

        for (const auto& parameterToChange : parametersToChange)
        {
            const auto parameterName = parameterToChange.name();
            const auto parameterValue = parameterToChange.value();

            if (parameterName == cameraConfig.fps)
            {
                auto fps = std::stod(parameterValue);
                status = GXSetFloat(m_hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, fps);
            }
            else if (parameterName == cameraConfig.TriggerSource)
            {

                auto i = findSecond(triggerSource, parameterValue);
                status = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, i);
            }
            else if (parameterName == cameraConfig.AcquisitionMode)
            {
                auto i = findSecond(acquisitionMode, parameterValue);
                status = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, i);
            }
            else if (parameterName == cameraConfig.TriggerMode)
            {
                auto i = findSecond(triggerMode, parameterValue);
                status = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, i);
            }
            else if (parameterName == cameraConfig.TriggerSelector)
            {
                auto i = findSecond(triggerSelector, parameterValue);
                status = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SELECTOR, i);
            }
            else if (parameterName == cameraConfig.LineSelector)
            {
                auto i = findSecond(lineSelector, parameterValue);
                status = GXSetEnum(m_hDevice, GX_ENUM_LINE_SELECTOR, i);
            }
            else if (parameterName == cameraConfig.PixelFormat)
            {
                auto i = findSecond(pixelFormat, parameterValue);
                status = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, i);
            }
            else if (parameterName == cameraConfig.Width)
            {
                auto i = std::stoi(parameterValue);
                status = GXSetInt(m_hDevice, GX_INT_WIDTH, i);
            }
            else if (parameterName == cameraConfig.Height)
            {
                auto i = std::stoi(parameterValue);
                status = GXSetInt(m_hDevice, GX_INT_HEIGHT, i);
            }

            if (status == GX_STATUS_SUCCESS)
                parameterStatuses.push_back(
                    CameraParameterStatus(parameterName, parameterValue, CameraParameterStatus::Status::OK));
            else
                parameterStatuses.push_back(
                    CameraParameterStatus(parameterName, parameterValue, CameraParameterStatus::Status::ERROR));
        }
        if (!isConnected())
        {
            closeCamera();
        }
        return parameterStatuses;
    }

    SDK::v1::CameraStatus DahengCamera::getStatus() const
    {
        const auto status =
            isConnected() ? SDK::v1::CameraStatus::Status::ONLINE : SDK::v1::CameraStatus::Status::OFFLINE;
        return SDK::v1::CameraStatus(status);
    }

} // namespace VCA::Daheng