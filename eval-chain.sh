#!/bin/bash

./generate-masks.py
./generate-disparity-maps.py
./evaluate-datasets.py
./aggregate-results.py
./aggregate-runtime.py
./calculate-mean-of-algorithms.py
