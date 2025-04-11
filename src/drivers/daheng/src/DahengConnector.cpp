#include "DahengConnector.h"

#include "DahengCamera.h"
#include "DahengPluginLogger.h"

#include <vector>

namespace VCA::Daheng
{
    DahengConnector::DahengConnector()
    {
        auto gxInitStatus = GXInitLib();
        if (gxInitStatus != GX_STATUS_SUCCESS)
        {
            throw VCA::SDK::v1::CameraException("Cannot initialize daheng library");
        }
    }

    DahengConnector::~DahengConnector()
    {
        GXCloseLib();
    }

    std::string DahengConnector::GetCameraString(GX_DEV_HANDLE hDevice, GX_FEATURE_ID feature) const
    {
        size_t nSize = 0;
        auto emStatus = GXGetStringLength(hDevice, feature, &nSize);
        if (emStatus != GX_STATUS_SUCCESS)
        {
            throw VCA::SDK::v1::CameraException("Error getting string lenght.");
        }

        std::vector<char> valString(nSize);
        emStatus = GXGetString(hDevice, feature, valString.data(), &nSize);
        if (emStatus != GX_STATUS_SUCCESS)
        {
            throw VCA::SDK::v1::CameraException("Error getting string value.");
        }

        return std::string(valString.data());
    }

    std::vector<std::string> DahengConnector::discover() const
    {
        std::vector<std::string> cameralist;

        // Get device enumerated number
        uint32_t ui32DeviceNum = 0;
        auto emStatus = GXUpdateDeviceList(&ui32DeviceNum, 1000);
        if (emStatus != GX_STATUS_SUCCESS)
        {
            throw VCA::SDK::v1::CameraException("Cannot discover the daheng devices.");
        }

        for (uint32_t i = 1; i <= ui32DeviceNum; ++i)
        {
            GX_DEV_HANDLE g_hDevice = NULL;

            emStatus = GXOpenDeviceByIndex(i, &g_hDevice);
            if (emStatus != GX_STATUS_SUCCESS)
            {
                continue;
            }

            std::string vendor = GetCameraString(g_hDevice, GX_STRING_DEVICE_VENDOR_NAME);
            std::string modelName = GetCameraString(g_hDevice, GX_STRING_DEVICE_MODEL_NAME);
            std::string serialNumber = GetCameraString(g_hDevice, GX_STRING_DEVICE_SERIAL_NUMBER);
            std::string deviceVersion = GetCameraString(g_hDevice, GX_STRING_DEVICE_VERSION);
            std::string userid = GetCameraString(g_hDevice, GX_STRING_DEVICE_USERID);
            cameralist.push_back(serialNumber);
        }

        return cameralist;
    }

    std::shared_ptr<VCA::SDK::v1::Camera> DahengConnector::createCamera(const std::string& cameraId) const
    {
        auto cam = std::make_shared<VCA::Daheng::DahengCamera>(cameraId);
        return cam;
    }
} // namespace VCA::Daheng