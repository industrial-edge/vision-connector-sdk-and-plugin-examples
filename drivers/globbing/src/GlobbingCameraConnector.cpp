#include "GlobbingCameraConnector.h"

#include "GlobbingCamera.h"
#include "GlobbingPluginLogger.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace VCA::Globbing
{
    GlobbingCameraConnector::GlobbingCameraConnector()
    {
        LOG_GLOBBING_PLUGIN_INFO("Camera connector initialized");
    }

    std::string getEnvVar(std::string key, std::string defaultValue)
    {
        const char* val = std::getenv(key.c_str());
        return val == NULL ? defaultValue : std::string(val);
    }

    std::vector<std::string> GlobbingCameraConnector::discover() const
    {
        std::string globbingDataFolder = getEnvVar("GLOBBING_DIR", "/globbing_data/");
        LOG_GLOBBING_PLUGIN_INFO("Using globbing directory: " + globbingDataFolder);

        std::vector<std::string> discoveredGlobbingDataFolders;

        if (fs::exists(globbingDataFolder))
        {
            for (const auto& entry : fs::directory_iterator(globbingDataFolder))
            {
                if (fs::is_directory(entry.path()))
                {
                    discoveredGlobbingDataFolders.push_back(entry.path());
                }
            }

            std::sort(discoveredGlobbingDataFolders.begin(), discoveredGlobbingDataFolders.end(),
                      [](const std::string& a, const std::string& b) { return a < b; });
        }
        else
        {
            LOG_GLOBBING_PLUGIN_INFO("Camera connector error: " + globbingDataFolder + " does not exist");
        }

        return discoveredGlobbingDataFolders;
    }

    std::shared_ptr<VCA::SDK::v1::Camera> GlobbingCameraConnector::createCamera(const std::string& cameraId) const
    {
        return std::make_shared<GlobbingCamera>(cameraId);
    }
} // namespace VCA::Globbing