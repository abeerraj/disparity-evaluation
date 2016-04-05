#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing
from functools import partial

config = {
    'cmd': '/Users/bjohn/git/thesis/disparity-evaluation/2_MaskCreator/bin/MaskCreator',
    'datasets': [
        {
            'path': '/Users/bjohn/desktop/datasets/cambridge/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
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

# execute algorithms
def execute(path, image):
    leftImagePath = os.path.join(path, 'left', image)
    dispLeftImagePath = os.path.join(path, 'disparity-left', image)
    dispRightImagePath = os.path.join(path, 'disparity-right', image)
    resultImagePath = os.path.join(path, 'masks')
    f = os.path.basename(resultImagePath)
    print 'processing ' + f
    # Usage: <left> <dispTruthLeft> <dispTruthRight> <out>
    subprocess.call([config['cmd'], leftImagePath, dispLeftImagePath, dispRightImagePath, resultImagePath])
    return

# create all the things
for d in config['datasets']:
    for s in d['sequences']:
        path = os.path.join(d['path'], s)
        mkdirs(os.path.join(path, 'masks'))
        print 'creating masks for sequence: ' + os.path.basename(path)
        start = time.time()
        images = getListOfImages(os.path.join(path, 'left')) # only read left directory
        for image in images:
            execute(path, image)
        end = time.time()
        delta = "%.2f" % (end - start)
        print delta + ' seconds runtime (sequence: ' + os.path.basename(path) + ')'

exit(0)
