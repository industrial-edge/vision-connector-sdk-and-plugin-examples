#include "GlobbingApi.h"

#ifndef TESTING
VCA::SDK::v1::PluginLogger* getPluginLogger()
{
    return &VCA::Globbing::logger;
}

VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::Globbing::GlobbingCameraConnector();
}
#endif