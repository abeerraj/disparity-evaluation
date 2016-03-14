#!/bin/bash
export OpenCV_DIR=/usr/local/opt/opencv3/share/OpenCV

cd 1_DisparityAlgorithm/bin
cmake .. -Wno-dev
make
cd ../..

cd 2_BitmaskCreator/bin
cmake .. -Wno-dev
make
cd ../..

cd 3_DisparityEvaluation/bin
cmake .. -Wno-dev
make
cd ../..
