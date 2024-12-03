#pragma once
#include <PluginLogger.h>

#define LOG_NUMBERS_PLUGIN_INFO(message) LOG_PLUGIN_INFO(VCA::Numbers::logger, message);
#define LOG_NUMBERS_PLUGIN_WARNING(message) LOG_PLUGIN_WARNING(VCA::Numbers::logger, message);
#define LOG_NUMBERS_PLUGIN_ERROR(message) LOG_PLUGIN_ERROR(VCA::Numbers::logger, message);

namespace VCA::Numbers
{
    inline VCA::SDK::v1::PluginLogger logger("Numbers");
}