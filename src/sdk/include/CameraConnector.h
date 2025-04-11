#pragma once
#include "Camera.h"

#include <memory>
#include <string>
#include <vector>

namespace VCA::SDK::v1
{
    class CameraConnector
    {
    public:
        explicit CameraConnector() = default;
        virtual ~CameraConnector() = default;

        virtual std::vector<std::string> discover() const = 0;
        virtual std::shared_ptr<Camera> createCamera(const std::string& cameraId) const = 0;
    };
} // namespace VCA::SDK::v1
