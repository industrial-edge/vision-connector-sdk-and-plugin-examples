#!/bin/bash

cd ../..

cmake --preset numbers-release
cmake --build --preset numbers-release-build --target install