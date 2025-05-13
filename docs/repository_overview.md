# Repository overview

The Vision Connector Drivers repository provides utility scripts, templates, examples, and the Vision Connector SDK to facilitate camera connector development. The following list describes the content of the ```src``` folder.

## requirements folder
The requirements folder contains installation scripts, both essential and optional, for development.

 - ```install_build_requirements.sh```: Installs a specific CMake version and packages required for building camera connectors.
 - ```install_test_requirements.sh```: Sets up the environment to build and run tests for camera connectors.
 - ```install_opencv.sh```: Contains a specific version of OpenCV with FFmpeg, also used by Vision Connector. These 3rd-party libraries enable creation of camera connectors using OpenCV.
 - ```install_clangd_language_server_16_0_2.sh```: Installs the language server.

## sdk folder
The Vision Connector Drivers repository includes a pre-built Vision Connector SDK. The library file, header files, and SDK version information are located in the sdk folder.

## drivers folder
This folder contains camera connector examples. You can refer to these connectors' implementation for additional insights into the overall concept.

## template folder
The template folder contains base implementations of essential classes to facilitate new camera connector setup and development. It includes additional files for testing and folders for camera vendor's API files. These files and folders should not be edited directly, as they are used by the ```scaffolding``` utility script when creating a new connector.

## scaffolding script
The scaffolding utility script, located in the repository root, helps set up and initiate new camera connector development.

```bash
./scaffolding.sh *connector name*
# The connector name must not contain any space or special characters.
```
Running the script creates a new folder in the drivers folder with the given name, based on the template folder. The script replaces filenames and file contents to reflect the new camera connector name.

For example, running:
```bash
./scaffolding.sh MyCamImplementation
```

Results in:
- Creation of ```**MyCamImplementation**``` folder under ```drivers```
- Source filenames starting with ```MyCamImplementation``` prefix
- Camera connector related classes with ```MyCamImplementation``` prefix
- Empty ```lib``` and ```include``` folders for vendor files
- Updated ```CMakeLists.txt``` with ```MyCamImplementation``` project and library
- Added ```build.sh``` script with ```MyCamImplementation``` build and install commands
- Updated root ```CMakePresets.json``` and ```CMakeLists.txt``` with new ```MyCamImplementation``` camera connector configurations

<b>Note: ```MyCamImplementation``` and ```MyCamImplementation``` are intentionally used separately. Vision Connector restricts the camera connector naming convention - the library file and connector name must contain only lowercase letters. The script handles this naming convention automatically.</b>

## build_drivers script
With a configured development environment, available camera connectors in the drivers folder can be built from source using the build_drivers script. This script builds the release version of connectors and packages them ready for use in Vision Connector.

## build_drivers_with_docker script
The ```build_drivers_with_docker``` scripts enable building available camera connectors from source without setting up the development environment, using Docker instead. The script uses the ```Dockerfile``` in the repository root to create a temporary development environment inside a container and executes the ```build_drivers.sh``` script.

## Prerequisites for the scripts
Important: Scripts provided by the Vision Connector Drivers repository may require elevated user rights. For example, run the scaffolding script as follows:

```bash
chmod 777 scaffolding.sh
sudo ./scaffolding.sh MyCamImplementation
```