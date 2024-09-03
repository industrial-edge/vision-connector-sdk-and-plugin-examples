#!/bin/bash

apt-get update
apt-get install -y wget unzip

mkdir /clangd_language_server/
cd /clangd_language_server/
wget https://github.com/clangd/clangd/releases/download/16.0.2/clangd-linux-16.0.2.zip
unzip clangd-linux-16.0.2.zip

source /root/.bashrc
export PATH=/clangd_language_server/clangd_16.0.2/bin/:${PATH}
echo "export PATH=\"$PATH\"" >> /root/.bashrc