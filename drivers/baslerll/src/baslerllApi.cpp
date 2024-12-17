#include "baslerllApi.h"

#ifndef TESTING
VCA::SDK::v1::PluginLogger* getPluginLogger()
{
    return &VCA::baslerll::logger;
}

VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::baslerll::baslerllConnector();
}
#endif