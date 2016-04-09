#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing
from functools import partial

config = {
    'algorithms': range(9),
    'cmd': '/Users/bjohn/git/thesis/disparity-evaluation/1_DisparityAlgorithm/bin/DisparityAlgorithm',
    'datasets': [
        {
            'path': '/Users/bjohn/Desktop/datasets/cambridge-vc-14/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
        {
            'path': '/Users/bjohn/Desktop/datasets/cambridge-vc-28/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
        {
            'path': '/Users/bjohn/Desktop/datasets/cambridge-vc-40/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
        #{
        #    'path': '/Users/bjohn/Desktop/datasets/tsukuba/',
        #    'sequences': ['01-tsukuba']
        #}
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

# execute algorithms
def execute(path, a, image):
    current = multiprocessing.current_process()
    worker = current._identity[0]
    leftImagePath = os.path.join(path, 'left', image)
    rightImagePath = os.path.join(path, 'right', image)
    resultImagePath = os.path.join(path, 'computed', str(a))
    resultImagePath = os.path.join(resultImagePath, os.path.splitext(image)[0] + '.exr')
    f = os.path.basename(resultImagePath)
    if os.path.isfile(resultImagePath):
        print 'skipping ' + f
        return
    print 'processing ' + f
    # Usage: <identifier> <algorithmId> <left> <right> <out>
    subprocess.call([config['cmd'], str(worker), str(a), leftImagePath, rightImagePath, resultImagePath])
    return

# setup pool for processing parallelism for one sequence
def createPool(path, a):
    print 'creating pool for sequence: ' + os.path.basename(path) + ', algorithm: ' + str(a)
    mkdirs(os.path.join(path, 'computed', str(a)))
    start = time.time()
    p = multiprocessing.Pool(2)
    f = partial(execute, path, a)
    images = getListOfImages(os.path.join(path, 'left')) # only read left directory
    p.map(f, images)
    p.close()
    p.join()
    end = time.time()
    delta = "%.2f" % (end - start)
    print delta + ' seconds runtime (sequence: ' + os.path.basename(path) + ', algorithm: ' + str(a) + ')'

# create all the things
for a in config['algorithms']:
    for d in config['datasets']:
        for s in d['sequences']:
            path = os.path.join(d['path'], s)
            createPool(path, a)

exit(0)
