# Development environment

## Setup environment
All Industrial Edge applications run on linux based OS. Development on Windows or MacOS is also supported, but the build needs to be made on Linux environment. Although there is no specific restriction regarding the used developer tools since the ```scaffolding``` and ```build``` scripts heavily utilize ```CMake``` and its presets it is recommended to use them as well.

To ease the environment setup process the installation scripts from the ```requirements``` folder can be used. Depending on the selected development environment the PATH, LIBRARY_PATH and LD_LIBRARY_PATH environment variables might need to be updated according to the ```Dockerfile``` ```COMMON_PATH``` and ```COMMON_LIB_PATH``` args.

Add the following lines to the ```~/.bashrc``` file to use the modified environment variables globally:
```
export LIBRARY_PATH="/usr/local/lib/:/Pylon/lib/":${LIBRARY_PATH}
export LD_LIBRARY_PATH="/usr/local/lib/:/Pylon/lib/":${LD_LIBRARY_PATH}
export PATH=/cmake-3.21.7-linux-x86_64/bin/:${PATH}
```

Use the ```install_build_requirements.sh``` and ```install_test_requirements.sh``` scripts to get the general build and test tools. If the custom camera connector needs OpenCV support install it with ```install_opencv.sh```.

## Verify environment
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