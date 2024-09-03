#include "GlobbingCamera.h"

#include "CameraException.h"

#include <opencv2/opencv.hpp>
#include <thread>

namespace VCA::EXAMPLES
{
    GlobbingCamera::GlobbingCamera(const std::string& url) : Camera(url) {}

    std::shared_ptr<VCA::SDK::v1::Image> GlobbingCamera::acquireImage()
    {
        if (m_imagePaths.empty())
        {
            throw VCA::SDK::v1::CameraException("Globbing camera error: No image to read");
        }

        increaseImageSequenceCounter();

        const auto& imagePath = m_imagePaths[m_index];

        cv::Mat rawImage = cv::imread(imagePath);
        if (rawImage.empty())
        {
            throw VCA::SDK::v1::CameraException("Globbing camera error: Unable to read image file: " + imagePath);
        }

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
        image->addCustomField("image_path", imagePath);

        ++m_index;
        if (m_index >= m_imagePaths.size())
        {
            m_index = 0;
            collectImages();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));

        return image;
    }

    void GlobbingCamera::startImageAcquisition()
    {
        collectImages();
        m_index = 0;
    }

    void GlobbingCamera::stopImageAcquisition() {}

    SDK::v1::CameraParameters GlobbingCamera::getConfig()
    {
        SDK::v1::CameraParameters cameraParameters;

        SDK::v1::CameraParameter fpsCameraParameter;
        fpsCameraParameter.setName("FPS");
        fpsCameraParameter.setValue(std::to_string(m_fps));
        fpsCameraParameter.setType("int");
        fpsCameraParameter.setDescription("FPS");
        fpsCameraParameter.setMin("1");
        fpsCameraParameter.setMax("1000");
        fpsCameraParameter.setReadOnly(false);

        cameraParameters.push_back(fpsCameraParameter);

        return cameraParameters;
    }

    SDK::v1::CameraParameterStatuses GlobbingCamera::setConfig(const SDK::v1::CameraParameters& parametersToChange)
    {
        SDK::v1::CameraParameterStatuses parameterStatuses;

        for (const auto& parameterToChange : parametersToChange)
        {
            const auto parameterName = parameterToChange.name();
            const auto parameterValue = parameterToChange.value();

            if (parameterName == "FPS")
            {
                m_fps = std::stoi(parameterValue);
            }

            parameterStatuses.push_back(SDK::v1::CameraParameterStatus(parameterName, parameterValue,
                                                                       SDK::v1::CameraParameterStatus::Status::OK));
        }

        return parameterStatuses;
    }

    SDK::v1::CameraStatus GlobbingCamera::getStatus() const
    {
        const auto status =
            isConnected() ? SDK::v1::CameraStatus::Status::ONLINE : SDK::v1::CameraStatus::Status::OFFLINE;
        return SDK::v1::CameraStatus(status);
    }

    const std::vector<std::string>& GlobbingCamera::imagePaths() const
    {
        return m_imagePaths;
    }

    void GlobbingCamera::collectImages()
    {
        try
        {
            m_imagePaths.clear();
            cv::glob(cameraUniqueId() + "/*.jpg", m_imagePaths);
        }
        catch (const std::exception& ex)
        {
            std::cout << "Globbing camera error: Failed to collect images: " << ex.what() << std::endl;
        }
    }
} // namespace VCA::EXAMPLES