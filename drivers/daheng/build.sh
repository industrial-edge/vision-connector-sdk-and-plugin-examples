#!/bin/bash

cd ../..

cmake --preset daheng-release
cmake --build --preset daheng-release-build --target install