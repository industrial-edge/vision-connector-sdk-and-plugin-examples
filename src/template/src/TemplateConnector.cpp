#include "TemplateConnector.h"

#include "TemplateCamera.h"
#include "TemplatePluginLogger.h"

namespace VCA::Template
{
    std::vector<std::string> TemplateConnector::discover() const
    {
        return {};
    }

    std::shared_ptr<VCA::SDK::v1::Camera> TemplateConnector::createCamera(const std::string& cameraId) const
    {
        return std::make_shared<VCA::Template::TemplateCamera>(cameraId);
    }
} // namespace VCA::Template