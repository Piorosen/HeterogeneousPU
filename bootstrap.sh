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
    
    # sudo apt install libabsl-dev libflatbuffers-dev

    # wget https://github.com/koenvervloesem/bazel-on-arm/releases/download/v4.2.2/bazel-4.2.2
    # chmod +x bazel-4.2.2

    # https://coral.ai/docs/accelerator/get-started/
    # echo "deb https://packages.cloud.google.com/apt coral-edgetpu-stable main" | sudo tee /etc/apt/sources.list.d/coral-edgetpu.list
    # curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
    # sudo apt-get update
    # sudo apt-get install -y libedgetpu1-std
    # sudo apt-get install libedgetpu1-max

    # bazel install bazel-3.7.2
    git clone https://github.com/koenvervloesem/bazel-on-arm
    cd bazel-on-arm
    sudo make requirements
    make bazel 
    sudo make install
    cd ..

    # build tensorflow
    sudo apt install bazel-3.7.2
    cd module/tensorflow
    git checkout a4dfb8d1a71385bd6d122e4f27f86dcebb96712d -b tf2.5
    # sudo apt install python3.8 python3.8-dev
    # bazel build --config=elinux_aarch64 -c opt //tensorflow/lite/c:libtensorflowlite_c.so
    # bazel build --config=elinux_aarch64 -c opt //tensorflow/lite:libtensorflowlite.so
    # sudo make install 

    cd module/flatbuffers && \
    git checkout 6df40a2471737b27271bdd9b900ab5f3aec746c7 -b tf1.12  && \
    mkdir build && cd build  && \
    cmake -G Ninja -DCMAKE_INSTALL_PREFIX=$(pwd)/../bin -DCMAKE_BUILD_TYPE=Release ..  && \
    ninja install 


    # install vcpkg 
    sudo apt install -y bison meson

    git clone https://github.com/microsoft/vcpkg
    ./vcpkg/bootstrap-vcpkg.sh
    cd vcpkg
    export VCPKG_FORCE_SYSTEM_BINARIES=1
    ./vcpkg install opencv4
    # # tensorflow lite build
    # git clone https://github.com/tensorflow/tensorflow.git
    # ./tensorflow/lite/tools/make/download_dependencies.sh
    # pip3 install --upgrade pip3
    # pip install setuptools openvino-dev

    wget https://github.com/Piorosen/HeterogeneousPU/releases/download/OpenVINO-2022-03/OpenVINO.tar.gz -P components && \
    tar -zxvf components/OpenVINO.tar.gz -C components
    echo "source $(pwd)/components/OpenVINO/setupvars.sh" >> ~/.bashrc
	echo "export LD_LIBRARY_PATH=/usr/lib/mali:$(pwd)/link:$$LD_LIBRARY_PATH:" >> ~/.bashrc


    sudo apt-get install clinfo

fi

echo "done"

# rm -rf components/l_openvino_toolkit_debian9_arm_2022.2.0.7713.af16ea1d79a_armhf.tgz
# rm -rf components/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz
