#include "CameraException.h"

#include "TemplateCamera.h"

namespace VCA::Template
{
    TemplateCamera::TemplateCamera(const std::string& cameraType) : Camera(cameraType)
    {
        
    }

    std::shared_ptr<VCA::SDK::v1::Image> TemplateCamera::acquireImage()
    {
        auto image = std::make_shared<SDK::v1::Image>();
        //Return image
        return image;
    }

    void TemplateCamera::startImageAcquisition() {}

    void TemplateCamera::stopImageAcquisition() {}

    SDK::v1::CameraParameters TemplateCamera::getConfig()
    {
        SDK::v1::CameraParameters cameraParameters;
        //Push paramaters
        return cameraParameters;
    }

    SDK::v1::CameraParameterStatuses TemplateCamera::setConfig(const SDK::v1::CameraParameters& parametersToChange)
    {
        SDK::v1::CameraParameterStatuses parameterStatuses;
        //Set parameters
        return parameterStatuses;
    }

    SDK::v1::CameraStatus TemplateCamera::getStatus() const
    {
        const auto status =
            isConnected() ? SDK::v1::CameraStatus::Status::ONLINE : SDK::v1::CameraStatus::Status::OFFLINE;
        return SDK::v1::CameraStatus(status);
    }

} // namespace VCA::Template