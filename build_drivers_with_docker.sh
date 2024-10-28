#!/bin/bash
set -e
set -o pipefail
IMAGENAME=driver_build_img:0.0.1

docker build . -t $IMAGENAME --target local_build_img
docker run --rm --v $PWD/installed_drivers:/vca-drivers/installed_drivers $IMAGENAME ./build_drivers.sh
