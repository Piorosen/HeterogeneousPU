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

run:
	cd build && ./chacha && cd ..

all: configure build
	echo "haha!"

clean:
	cd build && \
	rm -rf .cmake CMakeFiles .ninja* cmake_install.cmake CMakeCache.txt CPack* install_manifest.txt chacha


build:
	if ! [ -d "build" ]; then \
		mkdir build; \
	fi
	
	cd build && cmake .. -G Ninja \
		-DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
		-DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++ \
		-DCMAKE_BUILD_TYPE=Debug
		
	cd build && ninja install
		

configure: rknn 
	cp module/ArmCL/build/*.a link/
	cp module/ArmCL/build/*.so link/
	cp module/flatbuffers/bin/lib/*.a link/
	cp module/libedgetpu/throttled/aarch64/libedgetpu.so.1.0 link/libedgetpu.so
	sudo cp module/libedgetpu/tflite_prebuilt/*.so link/
	cp module/SimpleRKNN/bin/*.a link/
	cp module/SimpleRKNN/bin/*.so link/
	cp module/armnn_prebuilt/* link/
	# cd module/OpenVINO/build && make install
	
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


tensorflow:
	export ARMCC_PREFIX=${HOME}/toolchains/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu- && \
	export ARMCC_FLAGS="-funsafe-math-optimizations" && \
	cmake ../tensorflow_src/tensorflow/lite \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_C_COMPILER=${ARMCC_PREFIX}gcc \
		-DCMAKE_CXX_COMPILER=${ARMCC_PREFIX}g++ \
		-DCMAKE_C_FLAGS="${ARMCC_FLAGS}" \
		-DCMAKE_CXX_FLAGS="${ARMCC_FLAGS}" \
		-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
		-DCMAKE_SYSTEM_NAME=Linux \
		-DCMAKE_SYSTEM_PROCESSOR=armv8 \
		-DTFLITE_ENABLE_INSTALL=ON \
		-DCMAKE_FIND_PACKAGE_PREFER_CONFIG=ON \
		-Dabsl_DIR=$INS_PATH/lib/cmake/absl \
		-DEigen3_DIR=$INS_PATH/share/eigen3/cmake \
		-DFlatbuffers_DIR=$INS_PATH/lib/cmake/flatbuffers \
		-DNEON_2_SSE_DIR=$INS_PATH/lib/cmake/NEON_2_SSE \
		-Dcpuinfo_DIR=$INS_PATH/share/cpuinfo \
		-Druy_DIR=$INS_PATH/lib/cmake/ruy \
		--build .

		cmake -DCMAKE_C_COMPILER=${ARMCC_PREFIX}gcc \
			-DCMAKE_CXX_COMPILER=${ARMCC_PREFIX}g++ \
			-DCMAKE_C_FLAGS="${ARMCC_FLAGS}" \
			-DCMAKE_CXX_FLAGS="${ARMCC_FLAGS}" \
			-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
			-DCMAKE_SYSTEM_NAME=Linux \
			-DCMAKE_SYSTEM_PROCESSOR=armv8 \
			-DCMAKE_INSTALL_PREFIX=$HOME/tflitebuild \
			../tensorflow_src/tensorflow/lite

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
	echo "plz, insert ~/.bashrc under text"
	echo "source ${DIR}/components/OpenVINO/setupvars.sh" 
	echo "export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./link" 

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

originarmcl: 
	cd module/OriginArmCL && \
		scons Werror=0 debug=0 asserts=0 logging=0 neon=1 opencl=1 cppthreads=1 openmp=0 arch=armv8a example=0 -j$(shell nproc --all)



# clean:
# 	rm -rf module/OpenCV/build
# 	rm -rf link/OpenCV
	
# 	rm -rf module/SimpleRKNN/build
# 	rm -rf module/SimpleRKNN/bin

# 	rm -rf module/ArmCL/build
# 	rm -rf module/libArmCL/build
# 	rm -rf link/*.a link/*.so build bin
# 	rm -rf $(shell $$HOME)/intel/openvino_2022
