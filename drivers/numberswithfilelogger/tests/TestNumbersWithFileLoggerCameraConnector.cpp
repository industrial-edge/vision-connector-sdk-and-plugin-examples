#include "../src/NumbersWithFileLoggerConnector.h"
#include "../src/NumbersWithFileLoggerPluginLogger.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace fs = std::filesystem;

TEST(TestNumbersWithFileLoggerCameraConnector, testDiscover)
{
    VCA::NumbersWithFileLogger::NumbersWithFileLoggerConnector numbersWithFileLoggerConnector;
    const auto discoveredCameras = numbersWithFileLoggerConnector.discover();

    EXPECT_EQ(discoveredCameras.size(), 4);
    EXPECT_EQ(discoveredCameras.at(0), "red");
    EXPECT_EQ(discoveredCameras.at(1), "green");
    EXPECT_EQ(discoveredCameras.at(2), "blue");
    EXPECT_EQ(discoveredCameras.at(3), "black");
}

TEST(TestNumbersWithFileLoggerCameraConnector, testCreateCameras)
{
    VCA::NumbersWithFileLogger::NumbersWithFileLoggerConnector numbersWithFileLoggerConnector;

    const auto redCamera = numbersWithFileLoggerConnector.createCamera("red");
    const auto greenCamera = numbersWithFileLoggerConnector.createCamera("green");
    const auto blueCamera = numbersWithFileLoggerConnector.createCamera("blue");
    const auto blackCamera = numbersWithFileLoggerConnector.createCamera("black");

    EXPECT_EQ(redCamera->cameraUniqueId(), "red");
    EXPECT_EQ(greenCamera->cameraUniqueId(), "green");
    EXPECT_EQ(blueCamera->cameraUniqueId(), "blue");
    EXPECT_EQ(blackCamera->cameraUniqueId(), "black");
}

TEST(TestNumbersWithFileLoggerCameraConnector, testFileLogger)
{
    const auto filePath = "/logs/numbers_with_file_logger.txt";
    fs::remove(filePath);

    EXPECT_FALSE(fs::exists(filePath));
    EXPECT_TRUE(VCA::NumbersWithFileLogger::logger.callbacks().empty());

    VCA::NumbersWithFileLogger::NumbersWithFileLoggerConnector numbersWithFileLoggerConnector;
    EXPECT_EQ(VCA::NumbersWithFileLogger::logger.callbacks().size(), 1);

    LOG_NUMBERSWITHFILELOGGER_PLUGIN_INFO("info");
    LOG_NUMBERSWITHFILELOGGER_PLUGIN_WARNING("warning");
    LOG_NUMBERSWITHFILELOGGER_PLUGIN_ERROR("error");

    EXPECT_TRUE(fs::exists(filePath));

    std::stringstream fileContentBuffer;
    std::ifstream logFile(filePath);
    fileContentBuffer << logFile.rdbuf();
    const auto fileContent = fileContentBuffer.str();
    logFile.close();

    EXPECT_TRUE(fileContent.find("[NumbersWithFileLogger] info\n"
                                 "[NumbersWithFileLogger] warning\n"
                                 "[NumbersWithFileLogger] error") != std::string::npos);
}