#include "baslerConnector.h"
#include "baslerCamera.h"

namespace VCA::basler
{
    std::vector<std::string> baslerConnector::discover() const
    {
        std::vector<std::string> cameralist;
        PylonInitialize();
        CTlFactory& TlFactory = CTlFactory::GetInstance();
        ITransportLayer* pTl = TlFactory.CreateTl( BaslerGigEDeviceClass );
        DeviceInfoList_t devices;
        if ( 0 == pTl->EnumerateDevices( devices ) ) {
            throw VCA::SDK::v1::CameraException("No camera present!");
        }
        DeviceInfoList_t::const_iterator it;
        for ( it = devices.begin(); it != devices.end(); ++it ) {
            std::cout << it->GetFullName().c_str() << std::endl;
            cameralist.push_back(it->GetFullName().c_str());
        }
        PylonTerminate();
        return cameralist;
    }

    std::shared_ptr<VCA::SDK::v1::Camera> baslerConnector::createCamera(const std::string& cameraId) const
    {
        auto cam = std::make_shared<VCA::basler::baslerCamera>(cameraId);
        return cam;
    }
} // namespace VCA::basler