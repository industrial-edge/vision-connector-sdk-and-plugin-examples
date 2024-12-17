# Vision Connector - Plugin concept

## Concept overview

Vision Connector is an application which allows the connection of cameras to the Industrial Edge ecosystem.

As of version 1.1.0p it is possible to integrate any camera vendor's C++ API with simple implementation steps through custom camera connectors for VCA. To start the development of a camera connector the following are required:
<ul>
    <li>the API documentation of the camera vendor,</li>
    <li>the C++ headers and libraries of the camera API,</li>
    <li>the VCA Drivers repository with its utility tools.</li>
</ul>

To help the creation of custom camera connectors for VCA this document provides the followings:
<ul>
    <li>information about the VCA Drivers repository,</li>
    <li>a guide to setup the development environment,</li>
    <li>technical details of the VCA SDK and the provided interfaces,</li>
    <li>a walkthrough to create and use a custom connector.</li>
</ul>


## VCA Drivers repository

### Structure

The VCA Drivers repository is available with utility scripts, templates, examples and the VCA SDK itself to ease the development of the camera connectors.

#### requirements folder
The requirements folder contains installation script both essential and optional for development.

<li>install_build_requirements.sh: This script installs a specific CMake version and the packages purely required for building the camera connectors.</li>
<li>install_test_requirements.sh: This script sets up the environment to build and run tests for the camera connectors.</li>
<li>install_opencv.sh: The repository contains a specific version of OpenCV with FFmpeg which are also used by VCA. With these 3rd-party libraries installed it is possible to create camera connectors using OpenCV.</li>
<li>install_clangd_language_server_16_0_2.sh: (For Dev Container) If Dev Container development environment is used it is recommended to use the clangd language server. This script installs the language server.</li>

#### sdk folder
The VCA Drivers repository comes with a pre-built VCA SDK. The library file, the header files and information about the current SDK version is found in the sdk folder.

#### drivers folder
This folder contains camera connector examples. You may refer to the implementation of these connectors to get additional insights of the whole concept.

#### template folder
To ease the setup and development of a new camera connector there are template files with the base implementation of the essential classes. There are additional files for testing and folders for the camera vendor's API files. These files and folders in the template folder are not to be edited, they are used by the <b>scaffolding</b> utility script when creating a new connector.

#### scaffolding script
The scaffolding utility script is located in the root of the repository. As mentioned in the <b>template folder</b> section this script is meant to setup and help start the development of a new camera connector.

    ./scaffolding.sh *connector name*
    # The connector name must not contain any space or special characters.

After running the script a new folder will be created in the drivers folder with the given name. This new folder is based on the <b>template folder</b>. The script will replace the filenames and file contents to reflect the new camera connector name.

For example running the script

    ./scaffolding.sh MyCam

has the following effect:
<ul>
    <li>The <i>mycam</i> folder is created under drivers,</li>
    <li>the source filenames are starting with the <i>MyCam</i> prefix,</li>
    <li>the camera connector related classes have the <i>MyCam</i> prefix,</li>
    <li>empty lib and include folders are added for the vendor files,</li>
    <li>the CMakeLists.txt is updated with the <i>mycam</i> project and library,</li>
    <li>the build.sh script is added with <i>MyCam</i> build and install commands,</li>
    <li>the root CMakePresets.json and CMakeLists.txt are updated with the new configurations for <i>MyCam</i> camera connector.</li>
</ul>

<b>Note that <i>MyCam</i> and <i>mycam</i> are intentionally used separately. The naming convention of the camera connector is currently restricted by VCA. The library file and the connector name must contain only lowercase letters. The script handles the naming.</b>

#### build_drivers script
If the development environment is setup, the available camera connectors under the drivers folder can be built from source using the build_drivers script. The script will build the release version of the connectors and package them ready to use in VCA.

#### build_drivers_with_docker script
Through the <b>build_drivers_with_docker</b> scripts it is possible to build the available camera connectors under the drivers folder from source without setting up the development environment using Docker. The script uses the <b>Dockerfile</b> in the root of the repository to create a temporary development environment inside a container and executes the same <b>build_drivers.sh</b> script.

### Prerequisites for the scripts
It is important to note that the scripts provided by the VCA Drivers repository might need elevated user rights. For example run the scaffolding script like this:

```
chmod 777 scaffolding.sh
sudo ./scaffoldoing.sh MyCam
```


## Setup development environment
All Industrial Edge applications run on linux based OS. Development on Windows or MacOS is also supported, but the build needs to be made on Linux environment. Although there is no specific restriction regarding the used developer tools since the <b>scaffolding</b> and <b>build</b> scripts heavily utilize CMake and its presets it is recommended to use them as well.

To ease the environment setup process the installation scripts from the <b>requirements</b> folder can be used. Through Docker a pre-setup dev container configuration is also provided. Depending on the selected development environment the the PATH, LIBRARY_PATH and LD_LIBRARY_PATH environment variables might need to be updated according to the <b>Dockerfile</b> COMMON_PATH and COMMON_LIB_PATH args.

Add the following lines to the <i>~/.bashrc</i> file to use the modified environment variables globally:
```
export LIBRARY_PATH="/usr/local/lib/:/Pylon/lib/":${LIBRARY_PATH}
export LD_LIBRARY_PATH="/usr/local/lib/:/Pylon/lib/":${LD_LIBRARY_PATH}
export PATH=/cmake-3.21.7-linux-x86_64/bin/:${PATH}
```

Use the <b>install_build_requirements.sh</b> and <b>install_test_requirements.sh</b> scripts to get the general build and test tools. If the custom camera connector needs OpenCV support install it with <b>install_opencv.sh</b>.

### Verifying the development environment
To verify that the development environment was setup correctly run the tests of the Numbers camera connector example. Note that this example requires OpenCV as well.

In a new terminal navigate to the root of the VCA Drivers repository and run the following commands:
```
cmake --preset numbers-tests
cmake --build --preset numbers-tests-build
ctest --preset numbers-tests
```
At the end we should get this output:

    100% tests passed, 0 tests failed out of 9

If the example camera connector is built without error and the tests are running without fail we have the development environment.

## VCA SDK
### Overview
The VCA SDK is a separately built library which allows users to create their own camera connectors and use them with VCA. The VCA Drivers repository contains a pre-built version of this SDK with the necessary header files under the <b>sdk</b> folder. Every custom camera connector package has to contain the <b>libVCA-SDK.so</b> and the custom camera conenctor library has to link to this .so file.

By default the custom camera connector package creation and linkage is handled by CMake configuration and build scripts. More information about this in the <i>Creating and using a camera connector</i> section

### Classes
#### PluginLogger class
The SDK provides logging capabilities through the PluginLogger class. The PluginLogger can store registered callbacks with the actual logging implementation. VCA uses this concept to register its own logging logic but the camera connector can also register its own logging callback if needed.
If there is no registered callback then a fallback logger is used which writes to the standard output.

While the PluginLogger provides instance based macros for the different log levels it is not recommended to use them on their own. Each camera connector has its own PluginLogger instance and its specific log macros. Use these specific macros.


#### Object and ObjectContainer classes
The SDK contains some utility classes such as the Object and the ObjectContainer. These classes provide a generalized way to handle data between the custom camera connector and the VCA application and are not used individually.

<i>Essentially the Object represents a JSON object.</i>

#### CameraStatus class
Extends the ObjectContainer class to handle specificated data for camera status. It is used in the Camera abstract class.

#### CameraParameter class
Extends the ObjectContainer class to handle specificated data for camera parameters.

The camera parameter can have the following fields:
<ul>
    <li><b>Name</b> - The name of the parameter</li>
    <li><b>Description</b> - Description for the parameter</li>
    <li><b>Value</b> - Value of the parameter</li>
    <li><b>Min</b> - Minimum value of the parameter (if applicable)</li>
    <li><b>Max</b> - Maximum value of the parameter (if applicable)</li>
    <li><b>Increment</b> - Expected value increment (if applicable)</li>
    <li><b>Symbolics</b> - Predefined value options (if applicable)</li>
    <li><b>Type</b> - Type of the parameter</li>
    <li><b>ReadOnly</b> - Accessibility of the parameter</li>
</ul>

The camera parameter handles each field as string independent from the original value data type. The original data type is defined by the <b>type</b> field:
<ul>
    <li>bool</li>
    <li>int</li>
    <li>float</li>
    <li>string</li>
    <li>enum</li>
</ul>

#### CameraParameterStatus class
Extends the ObjectContainer class to handle specificated data for camera parameter changes.

Depending on the success of the parameter change the status can be either <b>OK</b> or <b>ERROR</b>.
If the status is <b>OK</b> then the value is the changed parameter value otherwise the error message.

#### Image class
The Image class is responsible for managing the image data retrieved from the cameras. VCA supports multipart images. An instance of the Image class consist of no, one or more image parts. By default the image instance does not contain any image part. The Image instance does not take over of the ownership of added image parts.

It is also possible to add metadata to each image part and custom fields to the image instance itself.

#### CameraException
Custom exception for camera related errors

### Abstract Classes
The actual behaviour of the custom camera connector is defined by the re-implementation of the CameraConnector and Camera abstract classes.

#### CameraConnector class
This class is responsible for the discovery of corresponding cameras and the unique ID specific camera creation.

##### Virtual methods
<li><b>discover</b></li>
The <b>discover</b> method has to return the list of available cameras as unique IDs.

</br>
<li><b>createCamera</b></li>
The <b>createCamera</b> method has to create an instance of the unique ID specified camera and returns it as shared pointer.

#### Camera class
This class is responsible for managing the underlying  camera (handling connection, modifying settings, retriving images). An instance of the Camera class can represent either a real/physical camera on the network or a simulated/software device.

A camera instance has to be created through the CameraConnector specified by the unique ID.

##### Virtual methods
<li><b>startImageAcquisition</b></li>
Has to handle connection to the underlying camera and prepare it for image acqusition.

</br>
<li><b>stopImageAcquisition</b></li>
Has to handle the disconnection of the underlying camera and free resource access.

</br>
<li><b>getStatus</b></li>
Has to return ONLINE or OFFLINE depending on the actual connection to the underlying camera and the state of the image retrieval.

</br>
<li><b>acquireImage</b></li>
Main logic for retrieving images from the actual camera. This method is called by higher level components on a separate thread. To avoid race condition resource access management is handled internally.

Has to return an instance of Image as shared pointer.

</br>
<li><b>getConfig</li></b>
Retrieves the actual configuration of the underlying camera. This method is called by higher level components on a separate thread. To avoid race condition resource access management is handled internally.

Has to return the list CameraParameters.

</br>
<li><b>setConfig</li></b>
Changes the configuration of the camera according to the parameter given CameraParameters. A CameraParameterStatus belongs to each changed camera parameter. To avoid race condition resource access management is handled internally.

Has to return the list CameraParameterStatuses.

</br>
<li><b>setVersion</li></b>
Sets the camera connector's internal version which then becomes part of the camera configuration. Default value: <b>Not Defined</b>


## Creating and using a custom camera connector
### Overview
In this walkthrough we will create an OpenCV based camera connector called <b>NumbersWithFileLogger</b> and load it into VCA. The cameras of this camera connector will give us colored images with increasing numbers written on them. As the name suggests we shall also implement our own logging logic as part of the camera connector.

This camera connector is also available under the drivers folder as an example.

The following guide has the assumption that the development environment is up, CMake is used as build tool and VSCode as IDE.

### Creating the connector
#### Scaffolding
Navigate to the root of the VCA Drivers repository.
First we need the skeleton of the new camera connector. For that we shall use the scaffolding script:
```
./scaffolding.sh NumbersWithFileLogger
```
After running the script we should have the root CMakeLists.txt and CMakePresets.json updated with the new configuration and the ./drivers/numberswithfilelogger folder created with the following structure:

- `drivers/`
    - `numberswithfilelogger/` - Parent folder of the new camera connector
        - `include/` - Copy camera vendor provided C++ header files here
            - `Place 3rd party include files here.txt`
        - `lib/` - Copy camera vendor provided library files here
            - `Place 3rd party libs here.txt`
        - `src/`
            - `NumbersWithFileLoggerApi.cpp`
            - `NumbersWithFileLoggerApi.h` - Contains the exposed functions used by VCA
            - `NumbersWithFileLoggerCamera.cpp` - Contains the main logic for the camera
            - `NumbersWithFileLoggerCamera.h`
            - `NumbersWithFileLoggerConnector.cpp` - Contains the main logic for camera discovery and camera creation
            - `NumbersWithFileLoggerConnector.h`
            - `NumbersWithFileLoggerPluginLogger.h` - Contains this camera connectors specific logging macros and the logger instance
    - `tests/`
        - `CMakeLists.txt` - CMakeLists.txt file with the test configuration
        - `testMain.cpp` - Entry point for the gtest based tests
    - `build.sh` - Build script for building and installing the release version this camera connector
    - `CMakeLists.txt` - Main CMakeLists.txt file of the camera connector

#### Preparing the 3rd-party dependencies
In this step we should copy every camera vendor provided header file in the include folder and library file in the lib folder. The header files are included automatically but the camera connector's CMakeLists.txt file has to be modified by hand to link the necessary 3rd-party libraries.

In our example we won't use these two folders but the CMakeLists.txt file has to be updated so the camera connector can use OpenCV. We add/modify the following lines:
```
find_package(OpenCV REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE opencv_core opencv_videoio ${LIBS})
```

The modified CMakeLists.txt file look like this in the affected areas:
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
Refer to the CMakeLists.txt files of the examples to get more insights how to structure these files.

#### Configure and build new custom camera connector
After the scaffolding new configuration and build presets are available. To continue the development of the camera connector we have to configure our new project. This can be done from the IDE or from the terminal by running one of these commands:
```
cmake --preset numberswithfilelogger-debug
```
or
```
cmake --preset numberswithfilelogger-release
```
depending on which build type is required.

Although the current NumbersWithFileLogger camera connector does not contain any camera related implementation we can still build the project because of the scaffolding. Again we can do this through the IDE or from the terminal by running one of these commands:
```
cmake --build --preset numberswithfilelogger-debug-build --target numberswithfilelogger
```
or
```
cmake --build --preset numberswithfilelogger-release-build --target numberswithfilelogger
```
depending on the previously selected configuration.
After this we should have the libnumberswithfilelogger.so file generated in the output directory.

#### Implementing NumbersWithFileLoggerConnector class
As mentioned before the CameraConnector class is responsible for discovering and creating cameras. The NumbersWithFileLoggerConnector derived class has to contain the camera specific logic for discovery and creation of the.

In this example we shall modify the ```discover``` method so it returns a list of colors as unique ids for the possible cameras.

Modified code:
```
std::vector<std::string> NumbersWithFileLoggerConnector::discover() const
{
    return {"red", "green", "blue", "black"};
}
```
After loading the finalized camera connector in VCA we shall see this list from UI.

#### Implementing NumbersWithFileLoggerCamera class
Compared to the CameraConnector the Camera class is more complex because of its overall functioning. That said first we shall define a few member variable so that we can track the state of the camera.

Updated NumbersWithFileLoggerCamera.h:
```
private:
    int m_width = 1920;
    int m_height = 1080;
    cv::Scalar m_backgroundColor;

    double m_fontScale = 1.0;
    int m_thickness = 2;
    int m_baseline = 0;
    int m_fps = 1;
```

With these variables we are defining a default state for the cameras and we are also able to change the behaviour of the cameras at runtime.

Next let's update the NumbersWithFileLoggerCamera.cpp file method-by-method.

Updated NumbersWithFileLoggerCamera.cpp constructor to set the unique id defined background color:
```
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

Now we have every variable set which are required to generate and acquire images. The updated acquireImage method looks like this:
```
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

With the exception of ```increaseImageSequenceCounter();``` which we use to update the displayed counter up until ```auto image = std::make_shared<SDK::v1::Image>();``` everything is OpenCV specific code. This part of the code varies from one camera vendor API to another.

The important part is the creation of ```imageData``` and ```imageDetail``` objects and adding them to the ```image``` as an image part.

The ```ImageData``` constructor takes two parameters, one is the actual image buffer and the second is the size of the buffer. In the ```ImageData``` constructor the buffer is copied and the copied buffer is managed internally.

We also need an ```ImageDetail``` object to add the image part to the ```image```. One ```ImageDetail``` object contains the crucial information of given image frame. To such information belong the ```CameraInformation```, ```ImageInformation``` and ```PTPInformation```.
The ```CameraInformation``` contains the camera unique id and the stream id.
The ```ImageInformation``` contains the frame count, the width and height of the frame and the pixel format. Although not every camera or use case supports Precision Time Protocol it is required to provide the ```PTPInformation``` for the ```ImageDetail``` object. In this example PTP is not used therefore <b>Disabled</b> state is used with the value <b>0</b>.

As we have both ```imageData``` and ```imageDetail``` objects we can add them to the ```image``` and we can return it as it has been successfully acquired.

With the current implementation the NumbersWithFileLoggerCamera can now give use images but only with the same, default configuration. We have to modify the ```getConfig``` and ```setConfig``` methods if we want to change the camera and the acquired images.

The ```getConfig``` has to return the list of camera supported parameters such as FPS, Width, Height etc... The parameter list varies from one camera to another even in with same brand. With getConfig the VCA UI can dynamically display the available configuration options of the camera.

The ```setConfig``` has to handle the configuration changes and return a list of changed configuration entries, in our case variables and their chage status.

The implementation of ```getConfig``` and ```setConfig``` is the following:
```
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

```
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

Now we can change the camera configuration from the VCA UI while using the camera.

#### Adding an internal logger
As the chosen name of the camera connector suggests it should also have an internal file logger. To do that shall declare a new function ```logToFile``` in the ```NumbersWithFileLoggerPluginLogger.h``` and define it in the ```NumbersWithFileLoggerPluginLogger.cpp```.

We add the following to the ```NumbersWithFileLoggerPluginLogger.h```
```
void logToFile(VCA::SDK::v1::PluginLogLevel level, const std::string& msg, const std::string& file, int line);
```

and to the ```NumbersWithFileLoggerPluginLogger.cpp```
```
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

Next we have to register this function with the logger. The most straightforward way to do that is to use the ```NumbersWithFileLoggerConnector``` class' constructor. As the connector is instantiated only once by VCA itself it is safe to use this constructor for initialization of resources or in our case registering the internal logger.

Updating the NumbersWithFileLoggerConnector constructor:
```
#include "NumbersWithFileLoggerPluginLogger.h"
...
NumbersWithFileLoggerConnector::NumbersWithFileLoggerConnector()
{
    logger.registerCallback("NumbersWithFileLogger Logger", logToFile);
    LOG_NUMBERSWITHFILELOGGER_PLUGIN_INFO("Camera connector initialized");
}
```

### Testing the connector
With the scaffolding a test preset is also created. In the IDE we have to switch to the ```numberswithfilelogger-tests``` configuration and build the test application. After this we can run the tests.

From terminal we have to run these three commands to configure, build and run the tests:
```
cmake --preset numberswithfilelogger-tests
cmake --build --preset numberswithfilelogger-tests-build
ctest --preset numberswithfilelogger-tests
```

### Installing the connector
When building the custom camera connector with release configuration we can chose <b>install</b> as the target. The <b>install</b> target is modified so the library file will be generated in the ```installed_drivers/camera_connector_name/``` folder. The libVCA-SDK.so and the content of the corresponding ```lib``` folder are copied in the same output directory.

Again, it can be done in the IDE by switching the configuration and building the appropriate target or from the terminal running the corresponding ```build.sh``` script or running these commands directly:
```
cmake --preset numberswithfilelogger-release
cmake --build --preset numberswithfilelogger-release-build --target install
```

This ensures that the final build has all of its dependencies.

### Packaging the connector
When dealing with custom camera connectors Vision Connector Application expects a .zip file containing all of the necessary files without the top-level directory.

It can be done using any zip tool but the provided utility scripts are also able to generate the final oackage file

If the development environment is available use the ```build_drivers.sh``` script otherwise use the ```build_drivers_with_docker``` scripts as they can manage their own temporary development environment.

At the end we should have the following folder structure:
- `installed_drivers/`
    - `numberswithfilelogger/`
        - `info.txt`
        - `libnumberswithfilelogger.so`
        - `libVCA-SDK.so`
    - `numberswithfilelogger.zip`

Now the `numberswithfilelogger.zip` file can be used in VCA.


### Using the connector in VCA
In the VCA Grid view we can add our new camera connector through the <b>Manage camera connectors</b> modal.

It is important that the given name must match the name of the custom camera connector library name. In our case the generated library file is ```libnumberswithfilelogger.so``` so the added connector name must be ```numberswithfilelogger```

After adding the new connector from the UI the application will initialize it in 10 seconds. After that the <b>Add camera</b> modal will list the new connector as well as the list of colors as discovered cameras.