DIR := $(shell pwd)

all: rknn armcl
	echo "haha!"
rknn:
	echo "work directory : $(DIR)"
	if [ -d "library/SimpleRKNN/build" ]; then \
		rm -rf library/SimpleRKNN/build; \
	fi
	if [ -d "library/SimpleRKNN/bin" ]; then \
  		rm -rf library/SimpleRKNN/bin; \
	fi

	cd library/SimpleRKNN && \
		mkdir build && \
		cd build && \
		cmake .. && \
		make install 

libarmcl: 
	if [ -d "library/libArmCL/build" ]; then  echo "done"; else cd library/libArmCL && scons Werror=0 debug=0 asserts=0 logging=0 neon=1 opencl=0 cppthreads=1 openmp=0 arch=armv8a -j8; fi 

armcl: libarmcl
	cp library/libArmCL/build/*.a library/ArmCL/external
	cp library/libArmCL/build/*.so library/ArmCL/external
	
	if [ -d "library/ArmCL/build" ]; then \
  		rm -rf library/ArmCL/build; \
	fi
	cd library/ArmCL && \
		mkdir build && \
		cd build && \
		cmake .. && \
		make
	
clean:
	cd library/SimpleRKNN && rm -rf build
	cd library/libArmCL/build
