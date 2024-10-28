#!/bin/bash

apt-get update
apt-get install -y \
    pkg-config \
    unzip

opencv_build_directory=/opencv_build
mkdir $opencv_build_directory
cp $(dirname "$(realpath "$0")")/opencv/* $opencv_build_directory
cd $opencv_build_directory

# ffmpeg
echo "Building and installing ffmpeg 7.1"
tar -xvf ffmpeg-7.1.tar.gz
cd ffmpeg-7.1
./configure --disable-gpl --enable-shared --disable-static --disable-debug --disable-doc --enable-pic --disable-ffmpeg \
    --disable-ffplay --disable-ffprobe --disable-x86asm
make -j$(nproc)
make install

cd $opencv_build_directory

# opencv
echo "Building and installing opencv 4.9.0"
unzip 98611_opencv___opencv_opencv_4_9_0_sourcecode.zip -d opencv-4.9.0
cd opencv-4.9.0
mkdir build && cd build
cmake -DWITH_FFMPEG=ON -DBUILD_LIST=core,videoio -DVIDEOIO_PLUGIN_LIST=ffmpeg ..
make -j$(nproc)
make install

rm -r $opencv_build_directory