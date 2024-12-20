#pragma once
#include "CameraConnector.h"
#include <pylon/PylonIncludes.h>

namespace VCA::baslerll
{
    class baslerllConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit baslerllConnector() = default;
        ~baslerllConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::baslerll
