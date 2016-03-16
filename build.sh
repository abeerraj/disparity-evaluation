#!/bin/bash
export OpenCV_DIR=/usr/local/opt/opencv3/share/OpenCV

function compile {
  cmake .. -Wno-dev
  make
}

blocks=(1_DisparityAlgorithm 2_BitmaskCreator 3_DisparityEvaluation)

for i in ${blocks[@]}; do
  cd $i/bin
  compile
  cd ../..
done
exit 0
