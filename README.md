# Vision Connector Drivers

## Table of Contents
* **[Introduction to VCA Drivers](#introduction-to-vca-drivers)**
* **[Repository structure](#repository-structure)**
    * **[requirements folder](#requirements-folder)**
    * **[sdk folder](#sdk-folder)**
    * **[drivers folder](#drivers-folder)**
    * **[template folder](#template-folder)**
    * **[scaffolding script](#scaffolding-script)**
    * **[build_drivers script](#build_drivers-script)**
    * **[Prerequisites for the scripts](#prerequisites-for-the-scripts)**
* **[Development environment](#development-environment)**
    * **[Setup environment](#setup-environment)**
    * **[Verify environment](#verify-environment)**
* **[VCA SDK](#vca-sdk)**
* **[Creating and using a custom camera connector](#creating-and-using-a-custom-camera-connector)**
* **[License and Legal Information](#license-and-legal-information)**
* **[Disclaimer](#disclaimer)**


## Introduction to VCA Drivers

The Industrial Edge Vision Connector App, or VCA for short combines industrial cameras with the Industrial Edge ecosystem. The Vision Connector uses an Ethernet interface to establish a connection to the cameras. The VCA publishes the data using the ZeroMQ server integrated in the Vision Connector with high throughput to other applications.

As of version 1.1.0 it is possible to integrate any camera vendor's C++ API with simple implementation steps through custom camera connectors for VCA. To start the development of a camera connector the following are required:
 - the API documentation of the camera vendor,
 - the C++ headers and libraries of the camera API,
 - the VCA Drivers repository with its utility tools.

<br>

To help the creation of custom camera connectors for VCA this project provides
 - information about the VCA Drivers repository,
 - a guide to setup the development environment,
 - technical details of the VCA SDK and the provided interfaces,
 - a walkthrough to create and use a custom connector.



## Repository structure

The VCA Drivers repository is available with utility scripts, templates, examples and the VCA SDK itself to ease the development of the camera connectors. The following list refers to the content of the ```src``` folder.

### requirements folder
The requirements folder contains installation scripts both essential and optional for development.

 - ```install_build_requirements.sh```: This script installs a specific CMake version and the packages purely required for building the camera connectors.
 - ```install_test_requirements.sh```: This script sets up the environment to build and run tests for the camera connectors.
 - ```install_opencv.sh```: The repository contains a specific version of OpenCV with FFmpeg which are also used by VCA. With these 3rd-party libraries installed it is possible to create camera connectors using OpenCV.
 - ```install_clangd_language_server_16_0_2.sh```: This script installs the language server.

### sdk folder
The VCA Drivers repository comes with a pre-built VCA SDK. The library file, the header files and information about the current SDK version is found in the sdk folder.

### drivers folder
This folder contains camera connector examples. You may refer to the implementation of these connectors to get additional insights of the whole concept.

### template folder
To ease the setup and development of a new camera connector there are template files with the base implementation of the essential classes. There are additional files for testing and folders for the camera vendor's API files. These files and folders in the template folder are not to be edited, they are used by the ```scaffolding``` utility script when creating a new connector.

### scaffolding script
The scaffolding utility script is located in the root of the repository. As mentioned in the template folder section this script is meant to setup and help start the development of a new camera connector.

```
./scaffolding.sh *connector name*
# The connector name must not contain any space or special characters.
```
After running the script a new folder will be created in the drivers folder with the given name. This new folder is based on the template folder. The script will replace the filenames and file contents to reflect the new camera connector name.

For example running the script

```
./scaffolding.sh MyCam
```

has the following effect:
- The ```mycam``` folder is created under ```drivers```,
- the source filenames are starting with the ```MyCam``` prefix,
- the camera connector related classes have the ```MyCam``` prefix,
- empty ```lib``` and ```include``` folders are added for the vendor files,
- the ```CMakeLists.txt``` is updated with the ```mycam``` project and library,
- the ```build.sh``` script is added with ```MyCam``` build and install commands,
- the root ```CMakePresets.json``` and ```CMakeLists.txt``` are updated with the new configurations for ```MyCam``` camera connector.

<b>Note that ```MyCam``` and ```mycam``` are intentionally used separately. The naming convention of the camera connector is currently restricted by VCA. The library file and the connector name must contain only lowercase letters. The script handles the naming.</b>

### build_drivers script
If the development environment is setup, the available camera connectors under the drivers folder can be built from source using the build_drivers script. The script will build the release version of the connectors and package them ready to use in VCA.

### build_drivers_with_docker script
Through the ```build_drivers_with_docker``` scripts it is possible to build the available camera connectors under the drivers folder from source without setting up the development environment using Docker. The script uses the ```Dockerfile``` in the root of the repository to create a temporary development environment inside a container and executes the same ```build_drivers.sh``` script.

### Prerequisites for the scripts
It is important to note that the scripts provided by the VCA Drivers repository might need elevated user rights. For example run the scaffolding script like this:

```
chmod 777 scaffolding.sh
sudo ./scaffoldoing.sh MyCam
```

## Development environment

### Setup environment
All Industrial Edge applications run on linux based OS. Development on Windows or MacOS is also supported, but the build needs to be made on Linux environment. Although there is no specific restriction regarding the used developer tools since the ```scaffolding``` and ```build``` scripts heavily utilize ```CMake``` and its presets it is recommended to use them as well.

To ease the environment setup process the installation scripts from the ```requirements``` folder can be used. Depending on the selected development environment the PATH, LIBRARY_PATH and LD_LIBRARY_PATH environment variables might need to be updated according to the ```Dockerfile``` ```COMMON_PATH``` and ```COMMON_LIB_PATH``` args.

Add the following lines to the ```~/.bashrc``` file to use the modified environment variables globally:
```
export LIBRARY_PATH="/usr/local/lib/:/Pylon/lib/":${LIBRARY_PATH}
export LD_LIBRARY_PATH="/usr/local/lib/:/Pylon/lib/":${LD_LIBRARY_PATH}
export PATH=/cmake-3.21.7-linux-x86_64/bin/:${PATH}
```

Use the ```install_build_requirements.sh``` and ```install_test_requirements.sh``` scripts to get the general build and test tools. If the custom camera connector needs OpenCV support install it with ```install_opencv.sh```.

### Verify environment
To verify that the development environment was setup correctly run the tests of the ```Daheng``` camera connector example.

In a new terminal navigate to the ```src``` folder of the VCA Drivers repository and run the following commands:
```
cmake --preset daheng-tests
cmake --build --preset daheng-tests-build
ctest --preset daheng-tests
```
At the end we should get this output:

```
100% tests passed, 0 tests failed out of 3
```

If the example camera connector is built without error and the tests are running without fail we have the development environment.

## VCA SDK
To get familiar with VCA SDK and the provided interfaces please read the [VCA SDK documentation](docs/vca_sdk.md)

## Creating and using a custom camera connector
The [Creating and using a custom camera connector](docs/creating_and_using_a_custom_camera_connector.md) documentation contains a walktrough to create a new camera connector from scratch and use it in VCA. 

## License and Legal Information

Please read the [Legal information](LICENSE.txt).

## Disclaimer

IMPORTANT - PLEASE READ CAREFULLY:

This documentation describes how you can download and set up containers which consist of or contain third-party software. By following this documentation you agree that using such third-party software is done at your own discretion and risk. No advice or information, whether oral or written, obtained by you from us or from this documentation shall create any warranty for the third-party software. Additionally, by following these descriptions or using the contents of this documentation, you agree that you are responsible for complying with all third party licenses applicable to such third-party software. All product names, logos, and brands are property of their respective owners. All third-party company, product and service names used in this documentation are for identification purposes only. Use of these names, logos, and brands does not imply endorsement.