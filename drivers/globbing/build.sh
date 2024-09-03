#!/bin/bash

cd ../..

cmake --preset globbing-release
cmake --build --preset globbing-release-build --target install