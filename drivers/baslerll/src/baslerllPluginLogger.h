#pragma once
#include <PluginLogger.h>

#define LOG_BASLERLL_PLUGIN_INFO(message) LOG_PLUGIN_INFO(VCA::baslerll::logger, message);
#define LOG_BASLERLL_PLUGIN_WARNING(message) LOG_PLUGIN_WARNING(VCA::baslerll::logger, message);
#define LOG_BASLERLL_PLUGIN_ERROR(message) LOG_PLUGIN_ERROR(VCA::baslerll::logger, message);

namespace VCA::baslerll
{
    inline VCA::SDK::v1::PluginLogger logger("Baslerll");
}