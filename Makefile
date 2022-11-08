DIR := $(shell pwd)
ARCH=$(shell uname -m)

ifeq ($(ARCH),x86_64)
PATH_COMPILER := $(DIR)/components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin
GCC_COMPILER := $(DIR)/components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu
else
PATH_COMPILER := /usr/bin
GCC_COMPILER := /usr/bin/aarch64-linux-gnu
endif

CYTHON_EXECUTABLE := /usr/bin/cython3
PYTHON_EXECUTABLE := /usr/bin/python3.7
PYTHON_LIBRARY := /usr/lib/aarch64-linux-gnu/libpython3.7m.so
PYTHON_INCLUDE_DIR := /usr/include/python3.7
CC := ${GCC_COMPILER}-gcc
CXX := ${GCC_COMPILER}-g++

.PHONY: all build test configure opencv openvino rknn armcl clean

test: 
	echo $(GCC_COMPILER)
	echo $(CC)
	echo $(CXX)
	echo $(shell $$HOME)

all: configure build
	echo "haha!"
	
build:
	if ! [ -d "build" ]; then \
		mkdir build; \
	fi
	
	cd build && cmake .. -G Ninja \
		-DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
		-DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++ \
		-DCMAKE_BUILD_TYPE=Release
		
	cd build && ninja install
		

configure: rknn armcl openvino
	cp module/ArmCL/build/*.a link/
	cp module/SimpleRKNN/bin/*.a link/
	cp module/ArmCL/build/*.so link/
	cp module/SimpleRKNN/bin/*.so link/
	cd module/OpenVINO/build && make install

opencv:
	if ! [ -d "module/OpenCV/build" ]; then \
		mkdir module/OpenCV/build; \
	fi
	if ! [ -d "link/OpenCV" ]; then \
		mkdir link/OpenCV; \
	fi

	cd module/OpenCV/build && \
	cmake .. \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
	-DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++  \
	-DCMAKE_CXX_FLAGS="-march=armv8-a" \
	-DCMAKE_INSTALL_PREFIX=${DIR}/components/OpenCV \
	-DCMAKE_TOOLCHAIN_FILE=${DIR}/module/OpenCV/platforms/linux/aarch64-gnu.toolchain.cmake && \
	make -j$(shell nproc --all) && \
	make install

	echo "export OpenCV_DIR=${DIR}/link/opencv" >> ~/.bashrc
	source ~/.bashrc

openvino:
	if ! [ -d "module/OpenVINO/build" ]; then \
		mkdir module/OpenVINO/build; \
	fi

	cd module/OpenVINO/build && \
	cmake .. \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
		-DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++  \
		-DCMAKE_CXX_FLAGS="-march=armv8-a" \
		-DTHREADS_PTHREAD_ARG="-pthread" \
		-DCMAKE_TOOLCHAIN_FILE="../cmake/arm64.toolchain.cmake" \
		-DCMAKE_INSTALL_PREFIX=components/OpenVINO \
		-DENABLE_MKL_DNN=OFF \
		-DENABLE_CLDNN=OFF \
		-DENABLE_GNA=OFF \
		-DENABLE_SSE42=OFF \
		-DENABLE_AVX512F=OFF \
		-DTHREADING=SEQ \
		-DNGRAPH_PYTHON_BUILD_ENABLE=ON \
		-DNGRAPH_ONNX_IMPORT_ENABLE=ON \
		-DENABLE_PYTHON=ON \
		-DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE} \
		-DPYTHON_LIBRARY=${PYTHON_LIBRARY} \
		-DCYTHON_EXECUTABLE=${CYTHON_EXECUTABLE} \
		-DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} && \
	make -j$(shell nproc --all)

rknn:
	echo "work directory : $(DIR)"
	if ! [ -d "module/SimpleRKNN/build" ]; then \
		mkdir module/SimpleRKNN/build; \
	fi
	if ! [ -d "module/SimpleRKNN/bin" ]; then \
  		mkdir module/SimpleRKNN/bin; \
	fi

	cd module/SimpleRKNN/build && \
		cmake .. \
		-DCMAKE_SYSTEM_NAME="Linux" \
		-DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
		-DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++ \
		-DCMAKE_BUILD_TYPE=Release && \
		make install 

armcl: 
	cd module/ArmCL && \
		scons Werror=0 debug=0 asserts=0 logging=0 neon=1 opencl=1 cppthreads=1 openmp=0 arch=armv8a example=0 -j$(shell nproc --all)


clean:
	rm -rf module/OpenCV/build
	rm -rf link/OpenCV
	
	rm -rf module/SimpleRKNN/build
	rm -rf module/SimpleRKNN/bin

	rm -rf module/ArmCL/build
	rm -rf module/libArmCL/build
	rm -rf link/*.a link/*.so build bin
	rm -rf $(shell $$HOME)/intel/openvino_2022
