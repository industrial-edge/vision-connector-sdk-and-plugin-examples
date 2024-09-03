#!/bin/bash
set -e
set -o pipefail
IMAGENAME=driver_build_img:0.0.1

docker build . -t $IMAGENAME --target build
docker run --rm -u $(id -u ${USER}):$(id -g ${USER}) -v $PWD/installed_drivers:/vca-drivers/installed_drivers build_test ./build_drivers.sh
