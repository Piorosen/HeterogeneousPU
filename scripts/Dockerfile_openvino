FROM debian:stretch

USER root

RUN apt update && \
    apt install libusb

RUN dpkg --add-architecture armhf && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    crossbuild-essential-armhf \
    git \
    wget \
    libusb-1.0-0-dev:armhf \
    libgtk-3-dev:armhf \
    libavcodec-dev:armhf \
    libavformat-dev:armhf \
    libswscale-dev:armhf \
    libgstreamer1.0-dev:armhf \
    libgstreamer-plugins-base1.0-dev:armhf \
    libpython3-dev:armhf \
    python3-pip \
    python-minimal \
    python-argparse \
    make \
    ninja-build \
    tar \
    scons

RUN wget https://github.com/Kitware/CMake/releases/download/v3.25.0-rc3/cmake-3.25.0-rc3-linux-x86_64.tar.gz && \
    tar -zxvf cmake-3.25.0-rc3-linux-x86_64.tar.gz --strip 1 -C /usr && \
    rm -rf cmake-3.25.0-rc3-linux-x86_64.tar.gz && \
    echo "export PATH=/HeterogenousPU/components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin:$PATH" >> ~/.bashrc && \
    source ~/.bashrc

# RUN git config --global user.name "Piorosen" && \
#     git config --global user.email "aoiakzto@naver.com"
