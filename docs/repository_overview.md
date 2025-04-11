# Repository overview

The VCA Drivers repository is available with utility scripts, templates, examples and the VCA SDK itself to ease the development of the camera connectors. The following list refers to the content of the ```src``` folder.

## requirements folder
The requirements folder contains installation scripts both essential and optional for development.

 - ```install_build_requirements.sh```: This script installs a specific CMake version and the packages purely required for building the camera connectors.
 - ```install_test_requirements.sh```: This script sets up the environment to build and run tests for the camera connectors.
 - ```install_opencv.sh```: The repository contains a specific version of OpenCV with FFmpeg which are also used by VCA. With these 3rd-party libraries installed it is possible to create camera connectors using OpenCV.
 - ```install_clangd_language_server_16_0_2.sh```: This script installs the language server.

## sdk folder
The VCA Drivers repository comes with a pre-built VCA SDK. The library file, the header files and information about the current SDK version is found in the sdk folder.

## drivers folder
This folder contains camera connector examples. You may refer to the implementation of these connectors to get additional insights of the whole concept.

## template folder
To ease the setup and development of a new camera connector there are template files with the base implementation of the essential classes. There are additional files for testing and folders for the camera vendor's API files. These files and folders in the template folder are not to be edited, they are used by the ```scaffolding``` utility script when creating a new connector.

## scaffolding script
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

## build_drivers script
If the development environment is setup, the available camera connectors under the drivers folder can be built from source using the build_drivers script. The script will build the release version of the connectors and package them ready to use in VCA.

## build_drivers_with_docker script
Through the ```build_drivers_with_docker``` scripts it is possible to build the available camera connectors under the drivers folder from source without setting up the development environment using Docker. The script uses the ```Dockerfile``` in the root of the repository to create a temporary development environment inside a container and executes the same ```build_drivers.sh``` script.

## Prerequisites for the scripts
It is important to note that the scripts provided by the VCA Drivers repository might need elevated user rights. For example run the scaffolding script like this:

```
chmod 777 scaffolding.sh
sudo ./scaffoldoing.sh MyCam
```