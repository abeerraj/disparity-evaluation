#!/bin/bash

./generate-disparity-maps.py
./evaluate-datasets.py
./aggregate-results.py
./aggregate-runtime.py
./calculate-mean-of-algorithms.py
