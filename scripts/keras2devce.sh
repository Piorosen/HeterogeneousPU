#!/bin/bash

# TEST
# ./onnx2device.sh "resnet18.onnx" 
echo "convert \"$1\" : model name "
echo "only input pb file name : \"$1.h5\""

if ! [ -d "$1" ]; then 
	mkdir $1;
fi
if ! [ -d "$1/tf" ]; then 
	mkdir $1/tf;
fi
if ! [ -d "$1/armcl" ]; then 
	mkdir $1/armcl;
fi

if ! [ -d "$1/myriad" ]; then 
	mkdir $1/myriad;
fi
if ! [ -d "$1/rknn" ]; then 
	mkdir $1/rknn;
fi
if ! [ -d "$1/tflite" ]; then 
	mkdir $1/tflite;
fi
if ! [ -d "$1/coral" ]; then 
	mkdir $1/coral;
fi

python3.7 ./convert/keras2pb.py -m "$1.h5" -o "$1/tf/pb"

# python3 ./convert/keras2armcl.py -m "$1.h5" -o "$1/armcl"
python3.7 ./convert/keras2coral.py -m "$1.h5" -o "$1/coral/saved_model.tflite"
python3.7 ./convert/keras2tflite.py -m "$1.h5" -o "$1/tflite/$1.tflite"
python3.7 ./convert/tflite2rknn.py -m "$1/tflite/$1.tflite" -o "$1/rknn/$1.rknn"
mo --saved_model_dir "$1/tf/pb" --output_dir "$1/myriad" --data_type FP16




# python3.7 ./convert/keras2armcl.py -m "$1/keras/$1.h5" -o "$1/armcl"
# python3.7 ./convert/keras2tflite.py -m "$1/tf/pb" -o "$1/tflite/$1.tflite"
# python3.7 ./convert/tflite2rknn.py -m "$1/tflite/$1.tflite" -o "$1/rknn/$1.rknn"

# mo --saved_model_dir $2 --output_dir $1/myriad --data_type FP16
# mv ./$1/myriad/saved_model.bin ./$1/myriad/$1.bin
# mv ./$1/myriad/saved_model.mapping ./$1/myriad/$1.mapping
# mv ./$1/myriad/saved_model.xml ./$1/myriad/$1.xml
# onnx-tf==1.5.0