#!/bin/bash

mkdir components

wget https://releases.linaro.org/components/toolchain/binaries/6.5-2018.12/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz -P components && \
    tar -xvf components/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz -C components

rm -rf components/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz
