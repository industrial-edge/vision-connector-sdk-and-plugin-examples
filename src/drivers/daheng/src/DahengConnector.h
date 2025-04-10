#pragma once
#include "GxIAPI.h"

#include <CameraConnector.h>

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
