#include "NumbersCameraConnector.h"

#include "NumbersCamera.h"

namespace VCA::EXAMPLES
{
    std::vector<std::string> NumbersCameraConnector::discover() const
    {
        return {"red", "green", "blue", "black"};
    }

    std::shared_ptr<VCA::SDK::v1::Camera> NumbersCameraConnector::createCamera(const std::string& cameraId) const
    {
        return std::make_shared<VCA::EXAMPLES::NumbersCamera>(cameraId);
    }
} // namespace VCA::EXAMPLES