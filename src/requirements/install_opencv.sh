#!/bin/bash

apt-get update
apt-get install -y \
    pkg-config \
    unzip

opencv_build_directory=/opencv_build
mkdir $opencv_build_directory
cp $(dirname "$(realpath "$0")")/oss/* $opencv_build_directory
cd $opencv_build_directory

# ffmpeg
echo "Building and installing ffmpeg 7.1"
unzip -d ffmpeg-7.1 ffmpeg-7.1_cleared.zip
cd ffmpeg-7.1
patch < changes.diff
chmod 777 -R *
./configure --enable-version3 --enable-shared --enable-pic --disable-static --disable-debug --disable-x86asm --disable-asm --disable-inline-asm \
    --disable-large-tests --disable-avfilter --disable-doc --disable-programs --disable-decoder=truemotion1
make -j$(nproc)
make install

cd $opencv_build_directory

# opencv
echo "Building and installing opencv 4.9.0"
unzip -d opencv-4.9.0 opencv-4.9.0_cleared.zip
cd opencv-4.9.0
mkdir build && cd build
/cmake-3.21.7-linux-x86_64/bin/cmake -DWITH_FFMPEG=ON -DBUILD_LIST=core,videoio -DVIDEOIO_PLUGIN_LIST=ffmpeg ..
make -j$(nproc)
make install

rm -r $opencv_build_directory