#!/bin/bash

echo "Scaffolding for $1"

# Installing JSON requirements

if ! apt list | grep -q "^jq.*installed"; then
    apt-get update
    apt-get install -y jq
fi


# Preparing folders for new driver

if [ -d "drivers/${1,,}" ]; then
        echo "Driver $1 already exists"
        exit -1
fi

cp template/ drivers/${1,,}/ -r
cd drivers/${1,,}
sed -i 's/template/'${1,,}'/' CMakeLists.txt
sed -i 's/template/'${1,,}'/' build.sh
cd src
for f in Template*; do mv "$f" $(echo "$f" | sed 's/^Template/'$1'/g'); done
find . -type f -exec sed -i 's/Template/'$1'/g' {} \;
cd ../tests
find . -type f -exec sed -i 's/Template/'$1'/g' {} \;


# Preparing CMake for new driver
cd ../../..

jq --arg DRIVER $1 --arg DRIVER_LOWER ${1,,} --arg DRIVER_UPPER ${1^^} '
    .configurePresets += [
        {"name":"\($DRIVER_LOWER)-debug","displayName":"\($DRIVER) Debug Config","inherits":"vca-drivers-base-config","cacheVariables":{"TARGET_DRIVER":$DRIVER_UPPER,"CMAKE_BUILD_TYPE":"Debug"}},
        {"name":"\($DRIVER_LOWER)-release","displayName":"\($DRIVER) Release Config","inherits":"vca-drivers-base-config","cacheVariables":{"TARGET_DRIVER":$DRIVER_UPPER,"CMAKE_BUILD_TYPE":"Release"}},
        {"name":"\($DRIVER_LOWER)-tests","displayName":"\($DRIVER) Tests Config","inherits":"vca-drivers-tests-base-config","cacheVariables":{"TARGET_DRIVER":$DRIVER_UPPER}}
    ]' CMakePresets.json > temp.CMakePresets.json && mv temp.CMakePresets.json CMakePresets.json

jq --arg DRIVER $1 --arg DRIVER_LOWER ${1,,} --arg DRIVER_UPPER ${1^^} '
    .buildPresets += [
        {"name":"\($DRIVER_LOWER)-debug-build","displayName":"\($DRIVER) Debug Build","configurePreset":"\($DRIVER_LOWER)-debug","targets":[$DRIVER_LOWER]},
        {"name":"\($DRIVER_LOWER)-release-build","displayName":"\($DRIVER) Release Build","configurePreset":"\($DRIVER_LOWER)-release","targets":[$DRIVER_LOWER, "install"]},
        {"name":"\($DRIVER_LOWER)-tests-build","displayName":"\($DRIVER) Tests Build","configurePreset":"\($DRIVER_LOWER)-tests","targets":["\($DRIVER_LOWER)-Test"]}
    ]' CMakePresets.json > temp.CMakePresets.json && mv temp.CMakePresets.json CMakePresets.json

jq --arg DRIVER $1 --arg DRIVER_LOWER ${1,,} '
    .testPresets += [
        {"name":"\($DRIVER_LOWER)-tests","displayName":"\($DRIVER) Tests","configurePreset":"\($DRIVER_LOWER)-tests"}
    ]' CMakePresets.json > temp.CMakePresets.json && mv temp.CMakePresets.json CMakePresets.json

echo -e "\nif(TARGET_DRIVER STREQUAL ${1^^})\n\tadd_subdirectory(drivers/${1,,})\nendif()" >> CMakeLists.txt


# Preparing CI for new driver

echo "
test_${1,,}_driver:
  extends: .driver_test_base
  variables:
    DRIVER_NAME: ${1,,}

build_${1,,}_driver:
  extends: .driver_build_base
  needs:
    - test_${1,,}_driver
  variables:
    DRIVER_NAME: ${1,,}" >> ".gitlab-ci.yml"


echo "Created on ${PWD}/drivers/${1,,}"