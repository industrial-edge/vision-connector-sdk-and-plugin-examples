#!/bin/bash

cd ../..

cmake --preset template-release
cmake --build --preset template-release-build --target install