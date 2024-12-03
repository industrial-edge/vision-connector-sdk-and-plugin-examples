#pragma once
#include <PluginLogger.h>

#define LOG_TEMPLATE_PLUGIN_INFO(message) LOG_PLUGIN_INFO(VCA::Template::logger, message);
#define LOG_TEMPLATE_PLUGIN_WARNING(message) LOG_PLUGIN_WARNING(VCA::Template::logger, message);
#define LOG_TEMPLATE_PLUGIN_ERROR(message) LOG_PLUGIN_ERROR(VCA::Template::logger, message);

namespace VCA::Template
{
    inline VCA::SDK::v1::PluginLogger logger("Template");
}