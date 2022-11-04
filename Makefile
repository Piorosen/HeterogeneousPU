DIR := $(shell pwd)
GCC_COMPILER := $(DIR)/components/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu
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


openvino:
	echo "vino build..."

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

	# if ! [ -d "module/ArmCL/build" ]; then \
	#  	cd module/ArmCL && \
	# 	scons Werror=0 debug=0 asserts=0 logging=0 neon=1 opencl=0 cppthreads=1 openmp=0 arch=armv8a -j8; \
	# fi

clean:
	rm -rf module/SimpleRKNN/build
	rm -rf module/SimpleRKNN/bin

	rm -rf module/ArmCL/build
	rm -rf module/libArmCL/build
	rm -rf link/*.a link/*.so build bin
