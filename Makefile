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
	
armcl:
	sc
	
clean:
	cd library/SimpleRKNN && rm -rf build
