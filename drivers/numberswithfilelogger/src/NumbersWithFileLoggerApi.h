#pragma once
#include "NumbersWithFileLoggerConnector.h"
#include "NumbersWithFileLoggerPluginLogger.h"

#ifndef TESTING
extern "C"
{
    VCA::SDK::v1::PluginLogger* getPluginLogger();
    VCA::SDK::v1::CameraConnector* createCameraConnector();
}
#endif