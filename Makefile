DIR := $(shell pwd)

all: 
	echo "haha!"
	

configure: rknn armcl
	

rknn:
	echo "work directory : $(DIR)"
	if ! [ -d "module/SimpleRKNN/build" ]; then \
		mkdir module/SimpleRKNN/build; \
	fi
	if ! [ -d "module/SimpleRKNN/bin" ]; then \
  		mkdir module/SimpleRKNN/bin; \
	fi

	cd module/SimpleRKNN/build && \
		cmake .. && \
		make install 

armcl: 
	cd module/ArmCL && \
		scons Werror=0 debug=0 asserts=0 logging=0 neon=1 opencl=0 cppthreads=1 openmp=0 arch=armv8a -j16

	# if ! [ -d "module/ArmCL/build" ]; then \
	#  	cd module/ArmCL && \
	# 	scons Werror=0 debug=0 asserts=0 logging=0 neon=1 opencl=0 cppthreads=1 openmp=0 arch=armv8a -j8; \
	# fi

clean:
	rm -rf module/SimpleRKNN/build
	rm -rf module/SimpleRKNN/bin

	rm -rf module/ArmCL/build
	rm -rf module/libArmCL/build
