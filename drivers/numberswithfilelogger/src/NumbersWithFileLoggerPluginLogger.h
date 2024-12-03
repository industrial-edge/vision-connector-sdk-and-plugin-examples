#pragma once
#include <PluginLogger.h>

#define LOG_NUMBERSWITHFILELOGGER_PLUGIN_INFO(message) LOG_PLUGIN_INFO(VCA::NumbersWithFileLogger::logger, message);
#define LOG_NUMBERSWITHFILELOGGER_PLUGIN_WARNING(message)                                                              \
    LOG_PLUGIN_WARNING(VCA::NumbersWithFileLogger::logger, message);
#define LOG_NUMBERSWITHFILELOGGER_PLUGIN_ERROR(message) LOG_PLUGIN_ERROR(VCA::NumbersWithFileLogger::logger, message);

namespace VCA::NumbersWithFileLogger
{
    inline VCA::SDK::v1::PluginLogger logger("NumbersWithFileLogger");

    void logToFile(VCA::SDK::v1::PluginLogLevel level, const std::string& msg, const std::string& file, int line);
} // namespace VCA::NumbersWithFileLogger