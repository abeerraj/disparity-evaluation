#!/bin/bash
export OpenCV_DIR=/usr/local/opt/opencv3/share/OpenCV

cd 1_DisparityAlgorithm/bin
cmake .. -Wno-dev
make
cd ../..
