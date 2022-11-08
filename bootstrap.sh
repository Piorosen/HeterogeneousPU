#!/bin/bash

echo "git submodule update --init --recursive"
git submodule update --init --recursive

echo "install : package dependency"
sudo apt install -y ninja-build make wget tar scons cython3

ARCH=$(uname -m)
if [ $ARCH == "x86_64" ]; then
    OS=$(uname -s)
    if [ $ARCH == "Linux" ]; then
        mkdir components
        wget https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz -P components && \
            tar -xvf components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz -C components

        wget https://github.com/Piorosen/HeterogeneousPU/releases/download/OpenVINO-2022-03/OpenVINO.tar.gz -P components && \
        tar -zxvf components/OpenVINO.tar.gz -C components

        echo "export PATH=$PATH:$(pwd)/components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin" >> ~/.bashrc
        source ~/.bashrc
    fi
    if [ $ARCH == "Darwin" ]; then
        python3.9 -m pip install --upgrade pip
        python3.9 -m pip install openvino
        python3.9 -m pip install openvino-dev
    fi
fi

if [ $ARCH == "aarch64" ]; then
    sudo -E apt update
    sudo -E apt-get install -y \
            build-essential \
            wget \
            libssl-dev \
            ca-certificates \
            git \
            pkg-config \
            libgflags-dev \
            zlib1g-dev \
            nlohmann-json-dev \
            unzip \
            libusb-1.0-0-dev

    wget https://github.com/Kitware/CMake/releases/download/v3.24.1/cmake-3.24.1-linux-aarch64.tar.gz -P components && \
        sudo tar -zxvf components/cmake-3.24.1-linux-aarch64.tar.gz --strip 1 -C /usr && \
        rm -rf components/cmake-3.24.1-linux-aarch64.tar.gz
    
    # pip3 install --upgrade pip3
    # pip install setuptools openvino-dev

    wget https://github.com/Piorosen/HeterogeneousPU/releases/download/OpenVINO-2022-03/OpenVINO.tar.gz -P components && \
    tar -zxvf components/OpenVINO.tar.gz -C components
    echo "source ${DIR}/components/OpenVINO/setupvars.sh" >> ~/.bashrc
	echo "export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./link" >> ~/.bashrc
fi

echo "done"

# rm -rf components/l_openvino_toolkit_debian9_arm_2022.2.0.7713.af16ea1d79a_armhf.tgz
# rm -rf components/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz
