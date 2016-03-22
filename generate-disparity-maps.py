#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing
from functools import partial

config = {
    'algorithms': range(9),
    'cmd': '/home/bjohn/thesis/disparity-evaluation/1_DisparityAlgorithm/bin/DisparityAlgorithm',
    'datasets': [
        {
            'path': '/home/bjohn/thesis/datasets/svddd/',
            'sequences': [
                '01-bunny', '02-bird', '03-butterfly', '04-flying', '05-testing', '06-sharpen',
                '07-falling', '08-apple', '09-rodent', '10-arrow', '11-kite', '12-lookout',
                '13-gliding', '14-field', '15-tree'
            ]
        }
    ]
}

def mkdirs(path):
    if not os.path.exists(path): os.makedirs(path)
    return

def getListOfImages(path):
    images = []
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f)): images.append(f)
    images.sort()
    return images

# <identifier> <algorithmId> <left> <right> <out>

# general folder structure:
# - subfolder stereo contains stereo images
# - subfolder computed/{algorithmId} contains computed disparity maps

# execute algorithms
def execute(path, a, image):
    current = multiprocessing.current_process()
    worker = current._identity[0]
    stereoImagePath = os.path.join(path, 'stereo', image)
    resultImagePath = os.path.join(path, 'computed', str(a))
    resultImagePath = os.path.join(resultImagePath, os.path.splitext(image)[0] + '.exr')
    f = os.path.basename(resultImagePath)
    if os.path.isfile(resultImagePath):
        print 'skipping ' + f
        return
    print 'processing ' + f
    subprocess.call([config['cmd'], str(worker), str(a), stereoImagePath, 'stereo', resultImagePath])
    return

# setup pool for processing parallelism for one sequence
def createPool(path, a):
    mkdirs(os.path.join(path, 'computed', str(a)))
    start = time.clock()
    p = multiprocessing.Pool(2)
    f = partial(execute, path, a)
    images = getListOfImages(os.path.join(path, 'stereo'))
    p.map(f, images)
    p.close()
    p.join()
    end = time.clock()
    print str(end - start) + ' seconds runtime (sequence: ' + os.path.basename(path) + ', algorithm: ' + str(a) + ')'

# create all the things
for a in config['algorithms']:
    for d in config['datasets']:
        for s in d['sequences']:
            path = os.path.join(d['path'], s)
            createPool(path, a)

exit(0)
