#!/bin/bash

apt-get update
apt-get install -y wget

cd /

wget https://github.com/Kitware/CMake/releases/download/v3.21.7/cmake-3.21.7-linux-x86_64.sh
chmod +x cmake-3.21.7-linux-x86_64.sh
yes | ./cmake-3.21.7-linux-x86_64.sh

apt-get install -y \
	build-essential \
    zip