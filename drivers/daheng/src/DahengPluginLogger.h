#pragma once
#include <PluginLogger.h>

#define LOG_DAHENG_PLUGIN_INFO(message) LOG_PLUGIN_INFO(VCA::Daheng::logger, message);
#define LOG_DAHENG_PLUGIN_WARNING(message) LOG_PLUGIN_WARNING(VCA::Daheng::logger, message);
#define LOG_DAHENG_PLUGIN_ERROR(message) LOG_PLUGIN_ERROR(VCA::Daheng::logger, message);

namespace VCA::Daheng
{
    inline VCA::SDK::v1::PluginLogger logger("Daheng");
}