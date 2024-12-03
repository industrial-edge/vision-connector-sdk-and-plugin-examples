#pragma once
#include "DahengConnector.h"
#include "DahengPluginLogger.h"

#ifndef TESTING
extern "C"
{
    VCA::SDK::v1::PluginLogger* getPluginLogger();
    VCA::SDK::v1::CameraConnector* createCameraConnector();
}
#endif