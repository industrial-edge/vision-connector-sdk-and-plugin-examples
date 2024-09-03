#include "TemplateConnector.h"
#include "TemplateCamera.h"

namespace VCA::Template
{
    std::vector<std::string> TemplateConnector::discover() const
    {
        return ;
    }

    std::shared_ptr<VCA::SDK::v1::Camera> TemplateConnector::createCamera(const std::string& cameraId) const
    {
        auto cam = std::make_shared<VCA::Template::TemplateCamera>(cameraId);
        return cam;
    }
} // namespace VCA::Template