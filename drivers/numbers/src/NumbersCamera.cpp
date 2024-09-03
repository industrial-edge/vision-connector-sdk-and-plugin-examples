#include "NumbersCamera.h"

#include "CameraException.h"

#include <chrono>
#include <string>
#include <thread>

namespace VCA::EXAMPLES
{
    NumbersCamera::NumbersCamera(const std::string& cameraType) : Camera(cameraType)
    {
        if (cameraType == "red")
        {
            m_backgroundColor = cv::Scalar(0, 0, 255);
        }
        else if (cameraType == "green")
        {
            m_backgroundColor = cv::Scalar(0, 255, 0);
        }
        else if (cameraType == "blue")
        {
            m_backgroundColor = cv::Scalar(255, 0, 0);
        }
        else
        {
            m_backgroundColor = cv::Scalar(0, 0, 0);
        }
    }

    std::shared_ptr<VCA::SDK::v1::Image> NumbersCamera::acquireImage()
    {
        increaseImageSequenceCounter();

        int fontFace = cv::FONT_HERSHEY_SIMPLEX;

        cv::Mat rawImage(m_height, m_width, CV_8UC3, m_backgroundColor);
        std::string number = std::to_string(imageSequenceCounter());

        cv::Size textSize = cv::getTextSize(number, fontFace, m_fontScale, m_thickness, &m_baseline);

        int x = (rawImage.cols - textSize.width) / 2;
        int y = (rawImage.rows + textSize.height) / 2;

        cv::putText(rawImage, number, cv::Point(x, y), fontFace, m_fontScale, cv::Scalar(255, 255, 255), m_thickness);

        const auto totalElementCount = rawImage.total() * rawImage.elemSize();
        const auto frameData = new uint8_t[totalElementCount];
        std::memcpy(frameData, reinterpret_cast<uint8_t*>(rawImage.data), totalElementCount);

        auto image = std::make_shared<SDK::v1::Image>();
        auto imageData = std::make_unique<SDK::v1::ImageData>(frameData, totalElementCount);
        const VCA::SDK::v1::ImageDetail imageDetail(
            VCA::SDK::v1::CameraInformation(cameraUniqueId(), "1"),
            VCA::SDK::v1::ImageInformation(imageSequenceCounter(), rawImage.cols, rawImage.rows, "BGR8"),
            VCA::SDK::v1::PTPInformation("Disabled", "0"));

        image->addImagePart(std::move(imageData), imageDetail);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));

        return image;
    }

    void NumbersCamera::startImageAcquisition() {}

    void NumbersCamera::stopImageAcquisition() {}

    SDK::v1::CameraParameters NumbersCamera::getConfig()
    {
        SDK::v1::CameraParameters cameraParameters;

        SDK::v1::CameraParameter widthCameraParameter;
        widthCameraParameter.setName("Width");
        widthCameraParameter.setValue(std::to_string(m_width));
        widthCameraParameter.setType("int");
        widthCameraParameter.setDescription("Width");
        widthCameraParameter.setReadOnly(false);

        SDK::v1::CameraParameter heightCameraParameter;
        heightCameraParameter.setName("Height");
        heightCameraParameter.setValue(std::to_string(m_height));
        heightCameraParameter.setType("int");
        heightCameraParameter.setDescription("Height");
        heightCameraParameter.setReadOnly(false);

        SDK::v1::CameraParameter fontScaleCameraParameter;
        fontScaleCameraParameter.setName("Font Scale");
        fontScaleCameraParameter.setValue(std::to_string(m_fontScale));
        fontScaleCameraParameter.setType("double");
        fontScaleCameraParameter.setDescription("Font Scale");
        fontScaleCameraParameter.setReadOnly(false);

        SDK::v1::CameraParameter thicknessCameraParameter;
        thicknessCameraParameter.setName("Thickness");
        thicknessCameraParameter.setValue(std::to_string(m_thickness));
        thicknessCameraParameter.setType("int");
        thicknessCameraParameter.setDescription("Thickness");
        thicknessCameraParameter.setReadOnly(false);

        SDK::v1::CameraParameter fpsCameraParameter;
        fpsCameraParameter.setName("FPS");
        fpsCameraParameter.setValue(std::to_string(m_fps));
        fpsCameraParameter.setType("int");
        fpsCameraParameter.setDescription("FPS");
        fpsCameraParameter.setMin("1");
        fpsCameraParameter.setMax("1000");
        fpsCameraParameter.setReadOnly(false);

        cameraParameters.push_back(widthCameraParameter);
        cameraParameters.push_back(heightCameraParameter);
        cameraParameters.push_back(fontScaleCameraParameter);
        cameraParameters.push_back(thicknessCameraParameter);
        cameraParameters.push_back(fpsCameraParameter);

        return cameraParameters;
    }

    SDK::v1::CameraParameterStatuses NumbersCamera::setConfig(const SDK::v1::CameraParameters& parametersToChange)
    {
        SDK::v1::CameraParameterStatuses parameterStatuses;

        for (const auto& parameterToChange : parametersToChange)
        {
            const auto parameterName = parameterToChange.name();
            const auto parameterValue = parameterToChange.value();

            if (parameterName == "Width")
            {
                m_width = std::stoi(parameterValue);
            }
            else if (parameterName == "Height")
            {
                m_height = std::stoi(parameterValue);
            }
            else if (parameterName == "Font Scale")
            {
                m_fontScale = std::stod(parameterValue);
            }
            else if (parameterName == "Thickness")
            {
                m_thickness = std::stoi(parameterValue);
            }
            else if (parameterName == "FPS")
            {
                m_fps = std::stoi(parameterValue);
            }

            parameterStatuses.push_back(SDK::v1::CameraParameterStatus(parameterName, parameterValue,
                                                                       SDK::v1::CameraParameterStatus::Status::OK));
        }

        return parameterStatuses;
    }

    SDK::v1::CameraStatus NumbersCamera::getStatus() const
    {
        const auto status =
            isConnected() ? SDK::v1::CameraStatus::Status::ONLINE : SDK::v1::CameraStatus::Status::OFFLINE;
        return SDK::v1::CameraStatus(status);
    }

} // namespace VCA::EXAMPLES