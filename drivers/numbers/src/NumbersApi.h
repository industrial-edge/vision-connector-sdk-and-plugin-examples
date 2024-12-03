#pragma once
#include "NumbersCameraConnector.h"
#include "NumbersPluginLogger.h"

#ifndef TESTING
extern "C"
{
    VCA::SDK::v1::PluginLogger* getPluginLogger();
    VCA::SDK::v1::CameraConnector* createCameraConnector();
}
#endif