#include "NumbersWithFileLoggerCamera.h"
#include "NumbersWithFileLoggerConnector.h"
#include "NumbersWithFileLoggerPluginLogger.h"

namespace VCA::NumbersWithFileLogger
{
    NumbersWithFileLoggerConnector::NumbersWithFileLoggerConnector()
    {
        logger.registerCallback("NumbersWithFileLogger Logger", logToFile);
        LOG_NUMBERSWITHFILELOGGER_PLUGIN_INFO("Camera connector initialized");
    }

    std::vector<std::string> NumbersWithFileLoggerConnector::discover() const
    {
        return {"red", "green", "blue", "black"};
    }

    std::shared_ptr<VCA::SDK::v1::Camera>
    NumbersWithFileLoggerConnector::createCamera(const std::string& cameraId) const
    {
        return std::make_shared<VCA::NumbersWithFileLogger::NumbersWithFileLoggerCamera>(cameraId);
    }
} // namespace VCA::NumbersWithFileLogger