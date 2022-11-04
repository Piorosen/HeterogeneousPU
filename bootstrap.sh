#!/bin/bash

echo "install : package dependency"
sudo apt install -y cmake ninja-build make wget tar scons

mkdir components
wget https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz -P components && \
    tar -xvf components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz -C components

echo "done"

# rm -rf components/l_openvino_toolkit_debian9_arm_2022.2.0.7713.af16ea1d79a_armhf.tgz
# rm -rf components/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz
