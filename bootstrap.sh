#!/bin/bash

mkdir components

docker run -d -p 8000:22 -v $(pwd):/workspaces -e PATH=/workspaces/components/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin -e LD_LIBRARY_PATH=./lib/ --name i_love_chacha aoikazto/sshd:ubuntu20.04

docker exec -it i_love_chacha apt update -y
docker exec -it i_love_chacha apt upgrade -y 
docker exec -it i_love_chacha apt install -y  make ninja-build cmake 

wget https://releases.linaro.org/components/toolchain/binaries/6.3-2017.02/aarch64-linux-gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu.tar.xz -P components && \
tar -zxvf components/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu.tar.xz -C components

rm -rf components/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu.tar.xz
