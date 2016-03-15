#!/bin/bash
export OpenCV_DIR=/usr/local/opt/opencv3/share/OpenCV

function compile {
  cmake .. -Wno-dev
  make
}

cd 1_DisparityAlgorithm/bin
compile
cd ../../2_BitmaskCreator/bin
compile
cd ../../3_DisparityEvaluation/bin
compile
cd../..
