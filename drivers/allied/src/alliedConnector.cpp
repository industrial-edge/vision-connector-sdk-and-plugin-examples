#include "alliedConnector.h"
#include "alliedCamera.h"

namespace VCA::allied
{
    std::vector<std::string> alliedConnector::discover() const
    {
        std::vector<std::string> cameralist;
        CameraPtr  m_camera;
        VmbSystem& vmbSystem = VmbSystem::GetInstance();
        VmbErrorType err = vmbSystem.Startup();

        if (err != VmbErrorSuccess){
            throw VCA::SDK::v1::CameraException("Could not start API, err=" + std::to_string(err));
        }

        CameraPtrVector cameras;
        err = vmbSystem.GetCameras(cameras);
        if (err != VmbErrorSuccess){
            vmbSystem.Shutdown();
            throw VCA::SDK::v1::CameraException("Could Not get cameras!");
        }

        if (cameras.empty()){
            vmbSystem.Shutdown();
            throw VCA::SDK::v1::CameraException("No camera present!");
        } else {
            for(int i = 0; i< cameras.size(); i++ ){
                std::string strID;
                cameras[i]->GetID(strID);
                cameralist.push_back(strID);
            }
        }
        vmbSystem.Shutdown();
        return cameralist;
    }

    std::shared_ptr<VCA::SDK::v1::Camera> alliedConnector::createCamera(const std::string& cameraId) const
    {
        auto cam = std::make_shared<VCA::allied::alliedCamera>(cameraId);
        return cam;
    }
} // namespace VCA::allied