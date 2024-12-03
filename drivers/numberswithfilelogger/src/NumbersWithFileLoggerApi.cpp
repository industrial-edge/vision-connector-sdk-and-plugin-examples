#include "NumbersWithFileLoggerApi.h"

#ifndef TESTING
VCA::SDK::v1::PluginLogger* getPluginLogger()
{
    return &VCA::NumbersWithFileLogger::logger;
}

VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::NumbersWithFileLogger::NumbersWithFileLoggerConnector();
}
#endif