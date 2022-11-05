#!/bin/bash

mv Dockerfile_openvino Dockerfile

sudo docker build -t ie_cross_armhf .
sudo docker run -it -v $(pwd)/..:/HeterogenousPU ie_cross_armhf "/bin/bash"
sudo docker exec -it $NAME make -C /HeterogenousPU openvino

sudo docekr rm $NAME

mv Dockerfile Dockerfile_openvino 
