#pragma once
#include <CameraConnector.h>

namespace VCA::NumbersWithFileLogger
{
    class NumbersWithFileLoggerConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit NumbersWithFileLoggerConnector();
        ~NumbersWithFileLoggerConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::NumbersWithFileLogger