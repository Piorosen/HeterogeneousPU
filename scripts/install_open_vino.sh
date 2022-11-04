#!/bin/bash

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

echo "source $HOME/intel/openvino_2022/setupvars.sh" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./link"
echo "done"
