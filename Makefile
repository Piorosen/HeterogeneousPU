DIR := $(shell pwd)
ARCH=$(shell uname -m)

ifeq ($(ARCH),x86_64)
GCC_COMPILER := $(DIR)/components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu
else
GCC_COMPILER := /usr/bin/aarch64-linux-gnu
endif

CC := ${GCC_COMPILER}-gcc
CXX := ${GCC_COMPILER}-g++


test: 
	echo $(GCC_COMPILER)
	echo $(CC)
	echo $(CXX)

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
	-DCMAKE_INSTALL_PREFIX=${DIR}/link/OpenCV \
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
		-DCMAKE_INSTALL_PREFIX=${DIR}/link/OpenVINO \
		-DENABLE_MKL_DNN=OFF \
		-DENABLE_CLDNN=OFF \
		-DENABLE_GNA=OFF \
		-DENABLE_SSE42=OFF \
		-DENABLE_AVX512F=OFF \
		-DTHREADING=SEQ && \
	make -j$(shell nproc --all)

	# chmod +x ./scripts/install_open_vino.sh
	# ./scripts/install_open_vino.sh

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
		scons Werror=0 debug=0 asserts=0 logging=0 neon=1 opencl=1 cppthreads=1 openmp=0 arch=armv8a -j16


clean:
	rm -rf module/OpenCV/build
	rm -rf link/OpenCV
	
	rm -rf module/SimpleRKNN/build
	rm -rf module/SimpleRKNN/bin

	rm -rf module/ArmCL/build
	rm -rf module/libArmCL/build
	rm -rf link/*.a link/*.so build bin
	rm -rf $(shell $HOME)/intel/openvino_2022
