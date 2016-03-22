#!/usr/bin/env python

import sys
import os
import subprocess
import time
from multiprocessing import Pool

#echo "dataset;frames;threshold;algorithm;outliers;rmse;" > results.csv

cmd = '/home/bjohn/thesis/disparity-evaluation/1_DisparityAlgorithm/bin/DisparityAlgorithm'
#<algorithmId> <left> <right> <out>

algorithms = range(9)

dataset = '/home/bjohn/thesis/datasets/svddd/'
#sequences = os.listdir(dataset)
#sequences.sort()

algorithm = '0'
sequence = '01-bunny'
resultPath = dataset + sequence + '/computed/' + algorithm + '/'
lookup = dataset + sequence + '/stereo/'
if not os.path.exists(resultPath):
    os.makedirs(resultPath)

#read all images
stereoImages = []
for f in os.listdir(lookup):
    if os.path.isfile(os.path.join(lookup, f)):
        stereoImages.append(f);
stereoImages.sort()

#execute algorithms
def execute(stereoImage):
    resultImage = os.path.splitext(stereoImage)[0] + '.exr'
    print stereoImage + " -> " + resultImage
    resultImage = os.path.join(resultPath, resultImage)
    stereoImage = os.path.join(lookup, stereoImage)
    subprocess.call([cmd, algorithm, stereoImage, 'stereo', resultImage])

start = time.clock()
#setup pool for processing parallelism
p = Pool(4)
p.map(execute, stereoImages)
p.close()
p.join()

end = time.clock()
print(end - start + "seconds runtime for the whole sequence")
