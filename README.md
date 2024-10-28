# VCA Drivers

This project contains the source code for VCA custom drivers with utility script for building the libraries.


## Develop locally

### Requirements
In the requirements folder there are install scripts to ease the environment setup.

- install_build_requirements.sh: This script installs a specific CMake version and the packages purely required for building the drivers
- install_opencv.sh: This script installs a specific version of OpenCV (4.9.0) with FFmpeg (7.1) which are also used by VCA
- install_test_requirements.sh: This script sets up the environment to run driver tests
- install_clangd_language_server_16_0_2.sh: (For Dev Container) If Dev Container development environment is used it is recommended to use the clangd language server. This script installs the language server


### Development environment

The drivers target build platform is Linux therefore it is recommended to use the same platform for development. To ease environment setup Dev Container is supported from VS Code but because of the install scripts developing directly on Linux or in virtual machine the setup just as easy. **If Linux machine or non-Docker virtualization is used the PATH, LIBRARY_PATH and LD_LIBRARY_PATH environment variables might need to be updated according to the Dockerfile COMMON_PATH and COMMON_LIB_PATH args**

Using the build_drivers_with_docker.bat and build_drivers_with_docker.sh scripts it is also possible to build and install the drivers through Docker without setting up the environment


## Scaffolding

The scaffolding.sh script allows users to easily create a new driver skeleton from template.

The script takes exactly one parameter which will be the name of the driver. After running the script it will create a new folder under the drivers directory, updates the CMakePresets.json and CMakeLists.txt files in the project root and for CI pipeline support new jobs are also added to the .gitlab-ci.yml file. If the given driver already exists it will do nothing. The CMakePresets.json file's configurationPresets section will be appended with a debug, release and test configuration of the new driver. Likewise the buildPresets section will be appended with debug, release and test build configuration with different targets. For CTest support the testPresets section is also appended with the corresponding test's configuration preset

Example: <br>
./scaffolding Globbing <br>
Folder: ./drivers/globbing <br>
Class name: GlobbingCameraConnector and GlobbingCamera <br>
CMake target: GLOBBING


## Gitlab CI

The project contains a .gitlab-ci file which is responsible to describe the driver test, driver build and driver publish stages. Each driver is tested and built separately and the successfully built drivers are published to captain artifactory on tagged versions