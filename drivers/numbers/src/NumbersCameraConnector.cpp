#include "NumbersCameraConnector.h"

#include "NumbersCamera.h"
#include "NumbersPluginLogger.h"

namespace VCA::Numbers
{
    NumbersCameraConnector::NumbersCameraConnector()
    {
        LOG_NUMBERS_PLUGIN_INFO("Camera connector initialized");
    }

    std::vector<std::string> NumbersCameraConnector::discover() const
    {
        return {"red", "green", "blue", "black"};
    }

    std::shared_ptr<VCA::SDK::v1::Camera> NumbersCameraConnector::createCamera(const std::string& cameraId) const
    {
        return std::make_shared<VCA::Numbers::NumbersCamera>(cameraId);
    }
} // namespace VCA::Numbers