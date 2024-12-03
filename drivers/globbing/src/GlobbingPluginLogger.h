#pragma once
#include <PluginLogger.h>

#define LOG_GLOBBING_PLUGIN_INFO(message) LOG_PLUGIN_INFO(VCA::Globbing::logger, message);
#define LOG_GLOBBING_PLUGIN_WARNING(message) LOG_PLUGIN_WARNING(VCA::Globbing::logger, message);
#define LOG_GLOBBING_PLUGIN_ERROR(message) LOG_PLUGIN_ERROR(VCA::Globbing::logger, message);

namespace VCA::Globbing
{
    inline VCA::SDK::v1::PluginLogger logger("Globbing");
}