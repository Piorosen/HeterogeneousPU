#!/bin/bash

set -e

RK3399PRO_TOOL_CHAIN=/workspaces/components/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu/
# for RK3399Pro aarch64
GCC_COMPILER=$RK3399PRO_TOOL_CHAIN/bin/aarch64-linux-gnu

ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

BUILD_DIR=${ROOT_PWD}/build

if [[ ! -d "${BUILD_DIR}" ]]; then
  mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake .. \
    -DCMAKE_SYSTEM_NAME="Linux" \
    -DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
    -DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++

make -j4

make install

cp run-board.sh bin/
