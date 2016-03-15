#!/bin/bash

blocks=(1_DisparityAlgorithm 2_BitmaskCreator 3_DisparityEvaluation)

for i in ${blocks[@]}; do
  rm -rf $i/bin/
  mkdir $i/bin/
done
exit 0
