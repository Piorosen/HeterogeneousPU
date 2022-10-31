#!/bin/bash

apt update -y && \
apt upgrade -y && \
apt install -y  make ninja-build cmake scons

# ArmCL 빌드

scons Werror=1 debug=0 asserts=0 \
        neon=1 opencl=1 \
        examples=0 \
        arch=arm-v8a -j8

