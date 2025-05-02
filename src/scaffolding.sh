#!/bin/bash

# Script to scaffold a new driver in the project

# Display the driver name being scaffolded
echo "Scaffolding for $1"

# Step 1: Install JSON requirements (jq is used for JSON processing)
if ! apt list | grep -q "^jq.*installed"; then
    apt-get update
    apt-get install -y jq
fi

# Step 2: Prepare folders for the new driver
# Check if the driver folder already exists
if [ -d "drivers/${1,,}" ]; then
    echo "Driver $1 already exists"
    exit -1
fi

# Copy the template folder to create a new driver folder
cp template/ drivers/${1,,}/ -r
cd drivers/${1,,}

# Replace occurrences of "template" with the new driver name in configuration files
sed -i 's/template/'${1,,}'/' CMakeLists.txt
sed -i 's/template/'${1,,}'/' build.sh

# Rename source files and update their content
cd src
for f in Template*; do 
    mv "$f" $(echo "$f" | sed 's/^Template/'$1'/g')
done
find . -type f -exec sed -i 's/Template/'$1'/g' {} \;
find . -type f -exec sed -i 's/TEMPLATE/'${1^^}'/g' {} \;

# Update test files with the new driver name
cd ../tests
find . -type f -exec sed -i 's/Template/'$1'/g' {} \;

# Step 3: Update CMake configuration for the new driver
cd ../../..

# Add new driver-specific configuration, build, and test presets to CMakePresets.json
for section in configurePresets buildPresets testPresets; do
    jq --arg DRIVER $1 --arg DRIVER_LOWER ${1,,} --arg DRIVER_UPPER ${1^^} --argjson presets "$(cat <<EOF
    {
        "configurePresets": [
            {"name":"\($DRIVER_LOWER)-debug","displayName":"\($DRIVER) Debug Config","inherits":"vca-drivers-base-config","cacheVariables":{"TARGET_DRIVER":$DRIVER_UPPER,"CMAKE_BUILD_TYPE":"Debug"}},
            {"name":"\($DRIVER_LOWER)-release","displayName":"\($DRIVER) Release Config","inherits":"vca-drivers-base-config","cacheVariables":{"TARGET_DRIVER":$DRIVER_UPPER,"CMAKE_BUILD_TYPE":"Release"}},
            {"name":"\($DRIVER_LOWER)-tests","displayName":"\($DRIVER) Tests Config","inherits":"vca-drivers-tests-base-config","cacheVariables":{"TARGET_DRIVER":$DRIVER_UPPER}}
        ],
        "buildPresets": [
            {"name":"\($DRIVER_LOWER)-debug-build","displayName":"\($DRIVER) Debug Build","configurePreset":"\($DRIVER_LOWER)-debug","targets":[$DRIVER_LOWER]},
            {"name":"\($DRIVER_LOWER)-release-build","displayName":"\($DRIVER) Release Build","configurePreset":"\($DRIVER_LOWER)-release","targets":[$DRIVER_LOWER, "install"]},
            {"name":"\($DRIVER_LOWER)-tests-build","displayName":"\($DRIVER) Tests Build","configurePreset":"\($DRIVER_LOWER)-tests","targets":["\($DRIVER_LOWER)-Test"]}
        ],
        "testPresets": [
            {"name":"\($DRIVER_LOWER)-tests","displayName":"\($DRIVER) Tests","configurePreset":"\($DRIVER_LOWER)-tests"}
        ]
    }
EOF
)" ".[$section] += \$presets[$section]" CMakePresets.json > temp.CMakePresets.json && mv temp.CMakePresets.json CMakePresets.json
done

# Add the new driver to the main CMakeLists.txt file
echo -e "\nif(TARGET_DRIVER STREQUAL ${1^^})\n\tadd_subdirectory(drivers/${1,,})\nendif()" >> CMakeLists.txt

# Print the location of the newly created driver
echo "Created on ${PWD}/drivers/${1,,}"