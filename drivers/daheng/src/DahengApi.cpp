#include "DahengApi.h"

#ifndef TESTING
VCA::SDK::v1::PluginLogger* getPluginLogger()
{
    return &VCA::Daheng::logger;
}

VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::Daheng::DahengConnector();
}
#endif