#!/bin/bash

cd ../..

cmake --preset numberswithfilelogger-release
cmake --build --preset numberswithfilelogger-release-build --target install