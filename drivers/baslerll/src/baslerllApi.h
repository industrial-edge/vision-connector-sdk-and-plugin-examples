#pragma once
#include "baslerllConnector.h"
#include "baslerllPluginLogger.h"

#ifndef TESTING
extern "C"
{
    VCA::SDK::v1::PluginLogger* getPluginLogger();
    VCA::SDK::v1::CameraConnector* createCameraConnector();
}
#endif