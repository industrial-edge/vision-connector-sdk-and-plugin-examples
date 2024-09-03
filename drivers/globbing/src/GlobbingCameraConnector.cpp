#include "GlobbingCameraConnector.h"

#include "GlobbingCamera.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace VCA::EXAMPLES
{
    std::string getEnvVar( std::string key, std::string defaultValue )
    {
        const char * val = std::getenv( key.c_str() );
        return val == NULL ? defaultValue : std::string(val);
    }

    std::vector<std::string> GlobbingCameraConnector::discover() const
    {
        std::string globbingDataFolder =  getEnvVar("GLOBBING_DIR", "/globbing_data/");
        std::cout << "Globbing directory " << globbingDataFolder << std::endl;
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
            std::cout << "Globbing camera connector error: " << globbingDataFolder << "does not exist" << std::endl;
        }

        return discoveredGlobbingDataFolders;
    }

    std::shared_ptr<VCA::SDK::v1::Camera> GlobbingCameraConnector::createCamera(const std::string& cameraId) const
    {
        return std::make_shared<GlobbingCamera>(cameraId);
    }
} // namespace VCA::EXAMPLES