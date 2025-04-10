#!/bin/bash

find ./drivers/ -name build.sh -exec sh -c 'cd $(dirname {}) && chmod +x ./build.sh && ./build.sh' \;
cd installed_drivers
find . -maxdepth 1 -type d ! -name '.' -exec sh -c "cd {} && zip -r ../{}.zip ." \;
