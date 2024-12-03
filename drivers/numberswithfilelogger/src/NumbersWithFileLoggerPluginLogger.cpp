#include "NumbersWithFileLoggerPluginLogger.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>

namespace fs = std::filesystem;

namespace VCA::NumbersWithFileLogger
{
    std::mutex file_mutex;
    void logToFile([[maybe_unused]] VCA::SDK::v1::PluginLogLevel level, const std::string& msg,
                   [[maybe_unused]] const std::string& file, [[maybe_unused]] int line)
    {
        try
        {
            const auto filePath = "/logs/numbers_with_file_logger.txt";
            fs::create_directories(fs::path(filePath).parent_path());

            std::ofstream file(filePath, std::ios::app);
            if (!file)
            {
                throw std::ios_base::failure("Failed to open log file");
            }

            file << msg << std::endl;
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }
} // namespace VCA::NumbersWithFileLogger