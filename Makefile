DIR := $(shell pwd)


all: rknn armcl
	echo "haha!"
rknn:
	echo "work directory : $(DIR)"
	cd library/SimpleRKNN && \
		mkdir build && \
		cd build && \
		cmake .. && \
		make install 
	
armcl:
	sc
	
clean:
	cd library/SimpleRKNN && rm -rf build