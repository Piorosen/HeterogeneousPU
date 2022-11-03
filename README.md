# ML-HeterogeneousPU

# Prerequirements

## Program

> 1. cmake < 3.4
> 2. [Cross-Compiler GCC 6.5.0](https://releases.linaro.org/components/toolchain/binaries/6.5-2018.12/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz)
> 3. ninja
> 4. make

## Architecture

> 1. CPU : Armv8a using NEON in ArmCL
> 2. GPU : Mali using OpenCL in ArmCL
> 3. NPU : Rockhip RK3399Pro using RKNN
> 4. NPU : Intel Neural Compute Stick 2 using OpenVINO

# How to build?

```sh
# Simple!
$ ./bootstrap.sh
$ make

# or some detail
$ ./bootstrap.sh
$ make rknn
$ make openvino
$ make armcl
$ make confgure
$ make build
```
