#include "NumbersApi.h"

#ifndef TESTING
VCA::SDK::v1::PluginLogger* getPluginLogger()
{
    return &VCA::Numbers::logger;
}

VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::Numbers::NumbersCameraConnector();
}
#endif