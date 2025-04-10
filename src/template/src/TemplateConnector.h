#pragma once
#include <CameraConnector.h>

namespace VCA::Template
{
    class TemplateConnector : public VCA::SDK::v1::CameraConnector
    {
    public:
        explicit TemplateConnector() = default;
        ~TemplateConnector() override = default;

        std::vector<std::string> discover() const override;
        std::shared_ptr<VCA::SDK::v1::Camera> createCamera(const std::string& cameraId) const override;
    };
} // namespace VCA::Template