#!/bin/bash

python3 ./convert/create_model.py -m $1
./keras2devce.sh $1
