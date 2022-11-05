#!/bin/bash

cd ..

wget https://github.com/opencv/opencv/archive/refs/tags/4.5.5-openvino-2022.1.0.tar.gz -P components && \
tar -zxvf components/4.5.5-openvino-2022.1.0.tar.gz -C module && \
mv module/opencv-4.5.5-openvino-2022.1.0 module/OpenCV

make opencv

cd scripts
