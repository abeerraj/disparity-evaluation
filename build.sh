#!/bin/bash

function clean {
  rm -rf $1/bin/
  mkdir $1/bin/
}

function compile {
  cd $1/bin
  cmake ..
  make -j4
  cd ../..
}

blocks=(1_DisparityAlgorithm 2_MaskCreator 3_DisparityEvaluation)
for i in ${blocks[@]}; do
  if [ "$1" == "clean" ]; then
    clean $i
  fi
  compile $i
done
exit 0
