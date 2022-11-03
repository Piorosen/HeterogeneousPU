#!/bin/bash

echo "install : package dependency"
sudo apt install -y cmake ninja-build make wget tar

mkdir components
wget https://releases.linaro.org/components/toolchain/binaries/6.5-2018.12/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz -P components && \
    tar -xvf components/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz -C components

# https://docs.openvino.ai/latest/openvino_docs_install_guides_installing_openvino_raspbian.html#install-openvino
ARCH=$(uname -m)
if [ $ARCH == "x86_64" ]; then
    echo "X86_64 : X86 Architecture"
    TOOL_KIT=l_openvino_toolkit_ubuntu20_2022.2.0.7713.af16ea1d79a_x86_64
else
    echo "AARCH 64 : ARM Architecture"
    TOOL_KIT=l_openvino_toolkit_debian9_arm_2022.2.0.7713.af16ea1d79a_armhf
fi

mkdir -p $HOME/intel/openvino_2022
wget https://storage.openvinotoolkit.org/repositories/openvino/packages/2022.2/linux/$TOOL_KIT.tgz -P components && \
tar -xvf components/$TOOL_KIT.tgz --strip 2 -C $HOME/intel/openvino_2022 && \
tar -xvf components/$TOOL_KIT.tgz --strip 1 -C $HOME/intel/openvino_2022 $TOOL_KIT/setupvars.sh

echo "done"

# rm -rf components/l_openvino_toolkit_debian9_arm_2022.2.0.7713.af16ea1d79a_armhf.tgz
# rm -rf components/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz
