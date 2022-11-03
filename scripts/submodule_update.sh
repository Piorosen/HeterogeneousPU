#!/bin/bash

P=$(pwd)
# echo $P
git submodule foreach git pull
cd $P
