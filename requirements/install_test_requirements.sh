#!/bin/bash

apt-get update
apt-get install -y \
	libgtest-dev \
	libgmock-dev \
	unzip \
	curl \
	git \
	zip \
	tar \
	pkg-config \
	unzip

cd /
git clone https://github.com/microsoft/vcpkg
/vcpkg/bootstrap-vcpkg.sh -disableMetrics
chmod -R a+rwx /vcpkg/
/vcpkg/vcpkg install nlohmann-json