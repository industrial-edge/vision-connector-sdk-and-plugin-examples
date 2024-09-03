#!/bin/bash


cd ../..

cmake --preset baslerll-release
cmake --build --preset baslerll-release-build --target install