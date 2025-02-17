#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#define LOG_PLUGIN_INFO(logger, message) logger.log(VCA::SDK::v1::PluginLogLevel::INFO, message, __FILE__, __LINE__);

#define LOG_PLUGIN_WARNING(logger, message)                                                                            \
    logger.log(VCA::SDK::v1::PluginLogLevel::WARNING, message, __FILE__, __LINE__);

#define LOG_PLUGIN_ERROR(logger, message) logger.log(VCA::SDK::v1::PluginLogLevel::ERROR, message, __FILE__, __LINE__);

namespace VCA::SDK::v1
{
    enum class PluginLogLevel
    {
        INFO,
        WARNING,
        ERROR
    };

    using LoggerCallback = void (*)(PluginLogLevel, const std::string&, const std::string&, int);

    class PluginLogger
    {
    public:
        explicit PluginLogger(const std::string& pluginName);
        ~PluginLogger() = default;

        PluginLogger(const PluginLogger&) = delete;
        PluginLogger(PluginLogger&&) = delete;
        PluginLogger& operator=(const PluginLogger&) = delete;
        PluginLogger& operator=(PluginLogger&&) = delete;

        const std::string& pluginName() const;

        bool registerCallback(const std::string& callbackName, LoggerCallback callback);
        bool deregisterCallback(const std::string& callbackName);
        const std::unordered_map<std::string, LoggerCallback>& callbacks() const;
        bool contains(const std::string& callbackName) const;

        void log(PluginLogLevel logLevel, const std::string& msg, const std::string& file, int line);

    private:
        std::string m_pluginName;
        std::unordered_map<std::string, LoggerCallback> m_callbacks;

        void fallbackLogToConsole(PluginLogLevel logLevel, const std::string& msg, const std::string& file, int line);

        std::string getCurrentTimestamp() const;
        std::string createMessageWithPluginPrefix(const std::string& msg) const;
        static std::string logLevelToString(PluginLogLevel logLevel);
    };

} // namespace VCA::SDK::v1
