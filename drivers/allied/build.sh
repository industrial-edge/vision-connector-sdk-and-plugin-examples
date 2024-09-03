#!/bin/bash


cd ../..

cmake --preset allied-release
cmake --build --preset allied-release-build --target install