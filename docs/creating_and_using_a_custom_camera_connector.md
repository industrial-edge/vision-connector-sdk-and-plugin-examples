# Creating and using a custom camera connector

## Table of Contents
* **[Overview](#overview)**
* **[Creating the connector](#creating-the-connector)**
    * **[Scaffolding](#scaffolding)**
    * **[Preparing the 3rd-party dependencies](#preparing-the-3rd-party-dependencies)**
    * **[Configure and build new custom camera connector](#configure-and-build-new-custom-camera-connector)**
    * **[Implementing NumbersWithFileLoggerConnector class](#implementing-numberswithfileloggerconnector-class)**
    * **[Implementing NumbersWithFileLoggerCamera class](#implementing-numberswithfileloggercamera-class)**
    * **[Adding an internal logger](#adding-an-internal-logger)**
* **[Testing the connector](#testing-the-connector)**
* **[Installing the connector](#installing-the-connector)**
* **[Packaging the connector](#packaging-the-connector)**
* **[Using the connector in Vision Connector](#using-the-connector-in-vision-connector)**

## Overview
In this walkthrough we will create an OpenCV based camera connector called ```NumbersWithFileLogger``` and load it into Vision Connector. The cameras of this camera connector will give us colored images with increasing numbers written on them. As the name suggests we shall also implement our own logging logic as part of the camera connector.

The following guide assumes that the development environment is set up, CMake is used as build tool and VSCode as IDE. It is important to set the working directory to the ```src``` folder located in the repository root.

## Creating the connector
### Scaffolding
Navigate to the ```src``` folder of the Vision Connector Drivers repository.
First we need the skeleton of the new camera connector. For that we shall use the ```scaffolding``` script:
```
./scaffolding.sh NumbersWithFileLogger
```
After running the script we should have the ```CMakeLists.txt``` and ```CMakePresets.json``` updated in the ```src``` folder with the new configuration and the ```src/drivers/numberswithfilelogger``` folder created with the following structure:

- `drivers/`
    - `numberswithfilelogger/` - Parent folder of the new camera connector
        - `include/` - Copy camera vendor provided C++ header files here
            - `Place 3rd party include files here.txt`
        - `lib/` - Copy camera vendor provided library files here
            - `Place 3rd party libs here.txt`
        - `src/`
            - `NumbersWithFileLoggerApi.cpp`
            - `NumbersWithFileLoggerApi.h` - Contains the exposed functions used by Vision Connector
            - `NumbersWithFileLoggerCamera.cpp` - Contains the main logic for the camera
            - `NumbersWithFileLoggerCamera.h`
            - `NumbersWithFileLoggerConnector.cpp` - Contains the main logic for camera discovery and camera creation
            - `NumbersWithFileLoggerConnector.h`
            - `NumbersWithFileLoggerPluginLogger.h` - Contains this camera connector's specific logging macros and the logger instance
    - `tests/`
        - `CMakeLists.txt` - CMakeLists.txt file with the test configuration
        - `testMain.cpp` - Entry point for the gtest based tests
    - `build.sh` - Build script for building and installing the release version this camera connector
    - `CMakeLists.txt` - Main CMakeLists.txt file of the camera connector

### Preparing the 3rd-party dependencies
In this step we should copy every camera vendor provided header file in the include folder and library file in the lib folder. The header files are included automatically but the camera connector's ```CMakeLists.txt``` file has to be modified by hand to link the necessary 3rd-party libraries.

In our example we won't use these two folders but the ```CMakeLists.txt``` file has to be updated so the camera connector can use OpenCV. We add/modify the following lines:
```
find_package(OpenCV REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE opencv_core opencv_videoio ${LIBS})
```

The modified CMakeLists.txt file looks like this in the affected areas:
```
...
include_directories(${SDK_INCLUDE_DIR})
link_directories(${SDK_LIBRARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)

if(TESTING STREQUAL "ON")
    add_subdirectory(tests)
endif()
...
```
```
...
add_library(${LIBRARY_NAME} SHARED ${PROJECT_SOURCES})

target_link_libraries(${LIBRARY_NAME} PRIVATE opencv_core opencv_videoio ${LIBS})
target_compile_options(${LIBRARY_NAME} PRIVATE -fno-gnu-unique)

install(TARGETS ${LIBRARY_NAME} DESTINATION ${LIBRARY_NAME})
...
```
Refer to the ```CMakeLists.txt``` files of the examples to get more insights on how to structure these files.

### Configure and build new custom camera connector
After the ```scaffolding```, new configuration and build presets are available. To continue the development of the camera connector we have to configure our new project. This can be done from the IDE or from the terminal by running one of these commands:
```
cmake --preset numberswithfilelogger-debug
```
or
```
cmake --preset numberswithfilelogger-release
```
depending on which build type is required.

Although the current ```NumbersWithFileLogger``` camera connector does not contain any camera related implementation we can still build the project because of the ```scaffolding```. Again we can do this through the IDE or from the terminal by running one of these commands:
```
cmake --build --preset numberswithfilelogger-debug-build --target numberswithfilelogger
```
or
```
cmake --build --preset numberswithfilelogger-release-build --target numberswithfilelogger
```
depending on the previously selected configuration.
After this we should have the ```libnumberswithfilelogger.so``` file generated in the output directory.

### Implementing NumbersWithFileLoggerConnector class
The ```CameraConnector``` class is responsible for discovering and creating cameras. The ```NumbersWithFileLoggerConnector``` derived class has to contain the camera specific logic for discovery and creation.

In this example we shall modify the ```discover``` method so it returns a list of colors as unique IDs for the possible cameras.

Modified code:
```
std::vector<std::string> NumbersWithFileLoggerConnector::discover() const
{
    return {"red", "green", "blue", "black"};
}
```
After loading the finalized camera connector in Vision Connector we shall see this list from UI.

### Implementing NumbersWithFileLoggerCamera class
Compared to the ```CameraConnector```, the ```Camera``` class is more complex because of its overall functioning. First, we shall define a few member variables so that we can track the state of the camera.

Updated NumbersWithFileLoggerCamera.h:
```cpp
private:
    int m_width = 1920;
    int m_height = 1080;
    cv::Scalar m_backgroundColor;

    double m_fontScale = 1.0;
    int m_thickness = 2;
    int m_baseline = 0;
    int m_fps = 1;
```

With these variables we are defining a default state for the cameras and we are also able to change the behavior of the cameras at runtime.

Next let's update the ```NumbersWithFileLoggerCamera.cpp``` file method-by-method.

Updated ```NumbersWithFileLoggerCamera.cpp``` constructor to set the unique ID defined background color:
```cpp
NumbersWithFileLoggerCamera::NumbersWithFileLoggerCamera(const std::string& uniqueId) : Camera(uniqueId)
{
    if (uniqueId == "red")
    {
        m_backgroundColor = cv::Scalar(0, 0, 255);
    }
    else if (uniqueId == "green")
    {
        m_backgroundColor = cv::Scalar(0, 255, 0);
    }
    else if (uniqueId == "blue")
    {
        m_backgroundColor = cv::Scalar(255, 0, 0);
    }
    else
    {
        m_backgroundColor = cv::Scalar(0, 0, 0);
    }
}
```

Now we have every variable set which is required to generate and acquire images. The updated acquireImage method looks like this:
```cpp
std::shared_ptr<VCA::SDK::v1::Image> NumbersWithFileLoggerCamera::acquireImage()
{
    increaseImageSequenceCounter();

    int fontFace = cv::FONT_HERSHEY_SIMPLEX;

    cv::Mat rawImage(m_height, m_width, CV_8UC3, m_backgroundColor);
    std::string number = std::to_string(imageSequenceCounter());

    cv::Size textSize = cv::getTextSize(number, fontFace, m_fontScale, m_thickness, &m_baseline);

    int x = (rawImage.cols - textSize.width) / 2;
    int y = (rawImage.rows + textSize.height) / 2;
    cv::putText(rawImage, number, cv::Point(x, y), fontFace, m_fontScale, cv::Scalar(255, 255, 255), m_thickness);

    const auto totalElementCount = rawImage.total() * rawImage.elemSize();
    auto image = std::make_shared<SDK::v1::Image>();
    auto imageData =
        std::make_unique<SDK::v1::ImageData>(reinterpret_cast<uint8_t*>(rawImage.data), totalElementCount);
    const VCA::SDK::v1::ImageDetail imageDetail(
        VCA::SDK::v1::CameraInformation(cameraUniqueId(), "1"),
        VCA::SDK::v1::ImageInformation(imageSequenceCounter(), rawImage.cols, rawImage.rows, "BGR8"),
        VCA::SDK::v1::PTPInformation("Disabled", "0"));

    image->addImagePart(std::move(imageData), imageDetail);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));

    return image;
}
```

With the exception of ```increaseImageSequenceCounter();``` which we use to update the displayed counter up until ```auto image = std::make_shared<SDK::v1::Image>();```, everything is OpenCV specific code. This part of the code varies from one camera vendor API to another.

The important part is the creation of ```imageData``` and ```imageDetail``` objects and adding them to the ```image``` as an image part.

The ```ImageData``` constructor takes two parameters: the actual image buffer and the size of the buffer. In the ```ImageData``` constructor, the buffer is copied and the copied buffer is managed internally.

We also need an ```ImageDetail``` object to add the image part to the ```image```. One ```ImageDetail``` object contains the crucial information of a given image frame. Such information includes the ```CameraInformation```, ```ImageInformation``` and ```PTPInformation```.
The ```CameraInformation``` contains the camera unique ID and the stream ID.
The ```ImageInformation``` contains the frame count, the width and height of the frame, and the pixel format. Although not every camera or use case supports Precision Time Protocol, it is required to provide the ```PTPInformation``` for the ```ImageDetail``` object. In this example, PTP is not used therefore **```Disabled```** state is used with the value **```0```**.

As we have both ```imageData``` and ```imageDetail``` objects, we can add them to the ```image``` and return it as it has been successfully acquired.

With the current implementation, the ```NumbersWithFileLoggerCamera``` can now give us images but only with the same, default configuration. We have to modify the ```getConfig``` and ```setConfig``` methods if we want to change the camera and the acquired images.

The ```getConfig``` has to return the list of camera supported parameters such as FPS, Width, Height etc. The parameter list varies from one camera to another even within the same brand. With ```getConfig``` the Vision Connector UI can dynamically display the available configuration options of the camera.

The ```setConfig``` has to handle the configuration changes and return a list of changed configuration entries, in our case variables and their change status.

The implementation of ```getConfig``` and ```setConfig``` is the following:
```cpp
SDK::v1::CameraParameters NumbersWithFileLoggerCamera::getConfig()
{
    SDK::v1::CameraParameters cameraParameters;

    SDK::v1::CameraParameter widthCameraParameter;
    widthCameraParameter.setName("Width");
    widthCameraParameter.setValue(std::to_string(m_width));
    widthCameraParameter.setType("int");
    widthCameraParameter.setDescription("Width");
    widthCameraParameter.setReadOnly(false);

    SDK::v1::CameraParameter heightCameraParameter;
    heightCameraParameter.setName("Height");
    heightCameraParameter.setValue(std::to_string(m_height));
    heightCameraParameter.setType("int");
    heightCameraParameter.setDescription("Height");
    heightCameraParameter.setReadOnly(false);

    SDK::v1::CameraParameter fontScaleCameraParameter;
    fontScaleCameraParameter.setName("Font Scale");
    fontScaleCameraParameter.setValue(std::to_string(m_fontScale));
    fontScaleCameraParameter.setType("double");
    fontScaleCameraParameter.setDescription("Font Scale");
    fontScaleCameraParameter.setReadOnly(false);

    SDK::v1::CameraParameter thicknessCameraParameter;
    thicknessCameraParameter.setName("Thickness");
    thicknessCameraParameter.setValue(std::to_string(m_thickness));
    thicknessCameraParameter.setType("int");
    thicknessCameraParameter.setDescription("Thickness");
    thicknessCameraParameter.setReadOnly(false);

    SDK::v1::CameraParameter fpsCameraParameter;
    fpsCameraParameter.setName("FPS");
    fpsCameraParameter.setValue(std::to_string(m_fps));
    fpsCameraParameter.setType("int");
    fpsCameraParameter.setDescription("FPS");
    fpsCameraParameter.setMin("1");
    fpsCameraParameter.setMax("1000");
    fpsCameraParameter.setReadOnly(false);

    cameraParameters.push_back(widthCameraParameter);
    cameraParameters.push_back(heightCameraParameter);
    cameraParameters.push_back(fontScaleCameraParameter);
    cameraParameters.push_back(thicknessCameraParameter);
    cameraParameters.push_back(fpsCameraParameter);

    return cameraParameters;
}
```

```cpp
SDK::v1::CameraParameterStatuses NumbersWithFileLoggerCamera::setConfig(const SDK::v1::CameraParameters& parametersToChange)
{
    SDK::v1::CameraParameterStatuses parameterStatuses;

    for (const auto& parameterToChange : parametersToChange)
    {
        const auto parameterName = parameterToChange.name();
        const auto parameterValue = parameterToChange.value();

        if (parameterName == "Width")
        {
            m_width = std::stoi(parameterValue);
        }
        else if (parameterName == "Height")
        {
            m_height = std::stoi(parameterValue);
        }
        else if (parameterName == "Font Scale")
        {
            m_fontScale = std::stod(parameterValue);
        }
        else if (parameterName == "Thickness")
        {
            m_thickness = std::stoi(parameterValue);
        }
        else if (parameterName == "FPS")
        {
            m_fps = std::stoi(parameterValue);
        }

        parameterStatuses.push_back(SDK::v1::CameraParameterStatus(parameterName, parameterValue, SDK::v1::CameraParameterStatus::Status::OK));
    }

    return parameterStatuses;
}
```

Now we can change the camera configuration from the Vision Connector UI while using the camera.

### Adding an internal logger
As the chosen name of the camera connector suggests it should also have an internal file logger. To do that we shall declare a new function ```logToFile``` in the ```NumbersWithFileLoggerPluginLogger.h``` and define it in the ```NumbersWithFileLoggerPluginLogger.cpp```.

We add the following to the ```NumbersWithFileLoggerPluginLogger.h```
```cpp
void logToFile(VCA::SDK::v1::PluginLogLevel level, const std::string& msg, const std::string& file, int line);
```

and to the ```NumbersWithFileLoggerPluginLogger.cpp```
```cpp
std::mutex file_mutex;
void logToFile([[maybe_unused]] VCA::SDK::v1::PluginLogLevel level, const std::string& msg, [[maybe_unused]] const std::string& file, [[maybe_unused]] int line)
{
    try
    {
        const std::lock_guard<std::mutex> lock(file_mutex);
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
```

Next we have to register this function with the logger. The most straightforward way to do that is to use the ```NumbersWithFileLoggerConnector``` class' constructor. As the connector is instantiated only once by Vision Connector itself, it is safe to use this constructor for initialization of resources or in our case registering the internal logger.

Updating the NumbersWithFileLoggerConnector constructor:
```cpp
#include "NumbersWithFileLoggerPluginLogger.h"
...
NumbersWithFileLoggerConnector::NumbersWithFileLoggerConnector()
{
    logger.registerCallback("NumbersWithFileLogger Logger", logToFile);
    LOG_NUMBERSWITHFILELOGGER_PLUGIN_INFO("Camera connector initialized");
}
```

## Testing the connector
With the scaffolding, a test preset is also created. In the IDE we have to switch to the ```numberswithfilelogger-tests``` configuration and build the test application. After this we can run the tests.

From terminal we have to run these three commands to configure, build and run the tests:
```bash
cmake --preset numberswithfilelogger-tests
cmake --build --preset numberswithfilelogger-tests-build
ctest --preset numberswithfilelogger-tests
```

## Installing the connector
When building the custom camera connector with release configuration we can choose **```install```** as the target. The **```install```** target is modified so the library file will be generated in the ```src/installed_drivers/camera_connector_name/``` folder. The ```libVCA-SDK.so``` and the content of the corresponding ```lib``` folder are copied in the same output directory.

Again, it can be done in the IDE by switching the configuration and building the appropriate target or from the terminal running the corresponding ```build.sh``` script or running these commands directly:
```bash
cmake --preset numberswithfilelogger-release
cmake --build --preset numberswithfilelogger-release-build --target install
```

This ensures that the final build has all of its dependencies.

## Packaging the connector
When dealing with custom camera connectors, Vision Connector Application expects a .zip file containing all of the necessary files without the top-level directory.

It can be done using any zip tool, but the provided utility scripts are also able to generate the final package file.

If the development environment is available use the ```build_drivers.sh``` script; otherwise use the ```build_drivers_with_docker``` scripts as they can manage their own temporary development environment.

At the end we should have the following folder structure:
- `installed_drivers/`
    - `numberswithfilelogger/`
        - `info.txt`
        - `libnumberswithfilelogger.so`
        - `libVCA-SDK.so`
    - `numberswithfilelogger.zip`

Now the `numberswithfilelogger.zip` file can be used in Vision Connector.

## Using the connector in Vision Connector
In the Vision Connector Grid view we can add our new camera connector through the <b>Manage camera connectors</b> modal.

It is important that the given name must match the name of the custom camera connector library name. In our case, the generated library file is ```libnumberswithfilelogger.so```, so the added connector name must be ```numberswithfilelogger```.

After adding the new connector from the UI, the application will initialize it in 10 seconds. After that, the <b>Add camera</b> modal will list the new connector as well as the list of colors as discovered cameras.