# Development environment

## Setup environment and conveinence scripts
All Industrial Edge applications run on Linux-based OS. Development on Windows or MacOS is also supported, but the build needs to be made in a Linux environment. Although there is no specific restriction regarding the developer tools, since the ```scaffolding``` and ```build``` scripts heavily utilize ```CMake``` and its presets, it is recommended to use them as well.

To ease the environment setup process, the installation scripts from the ```requirements``` folder can be used. Depending on the selected development environment, the LIBRARY_PATH, and LD_LIBRARY_PATH environment variables might need to be updated according to the ```Dockerfile``` ```COMMON_LIB_PATH``` arg.

Add the following lines to the ```~/.bashrc``` file to use the modified environment variables globally:
```bash
export LIBRARY_PATH="/usr/local/lib/":${LIBRARY_PATH}
export LD_LIBRARY_PATH="/usr/local/lib/":${LD_LIBRARY_PATH}
```
The following convience scripts are provieded in src/requirements for installing the general build tools, test tools, and OpenCV.
```
sudo ./install_build_requirements.sh
sudo ./install_test_requirements.sh
sudo ./install_opencv.sh
```
Please note that these scripts were designed for Debian 11 and root permission is required to exeucte them.

If you are using WSL then restarting it might be necessary after running the install scripts.


## Verify environment
To verify that the development environment was set up correctly, run the tests of the ```Daheng``` camera connector example.

In a new terminal, navigate to the ```src``` folder of the Vision Connector Drivers repository and run the following commands:
```bash
cmake --preset daheng-tests
cmake --build --preset daheng-tests-build
ctest --preset daheng-tests
```
At the end we should get this output:

```bash
100% tests passed, 0 tests failed out of 3
```

If the example camera connector is built without errors and the tests are running without failures, we have successfully set up the development environment.