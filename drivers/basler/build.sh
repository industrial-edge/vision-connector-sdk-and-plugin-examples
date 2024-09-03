#!/bin/bash


cd ../..

cmake --preset basler-release
cmake --build --preset basler-release-build --target install