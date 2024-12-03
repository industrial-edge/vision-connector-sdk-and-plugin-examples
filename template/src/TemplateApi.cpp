#include "TemplateApi.h"

#ifndef TESTING
VCA::SDK::v1::PluginLogger* getPluginLogger()
{
    return &VCA::Template::logger;
}

VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::Template::TemplateConnector();
}
#endif