#pragma once
#include "CameraConnector.h"
#include "GxIAPI.h"

namespace VCA::Daheng
{
    class DahengConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit DahengConnector();
        ~DahengConnector() override;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;

    private:
        std::string GetCameraString(GX_DEV_HANDLE hDevice, GX_FEATURE_ID feature) const;
    };
} // namespace VCA::Daheng

#ifndef TESTING
extern "C" VCA::SDK::v1::CameraConnector* createCameraConnector()
{
    return new VCA::Daheng::DahengConnector();
}
#endif