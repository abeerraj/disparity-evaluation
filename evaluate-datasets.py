#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing

config = {
    'algorithms': range(6),
    'cmd': '/Users/bjohn/git/thesis/disparity-evaluation/3_DisparityEvaluation/bin/DisparityEvaluation',
    'datasets': [
        {
            'path': '/Users/bjohn/Desktop/datasets/svddd/',
            'sequences': ['03-apple']
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
def execute(a, path, image):
    dispImagePath = os.path.join(path, 'computed', str(a), os.path.splitext(image)[0] + '.exr')
    dispTruthImagePath = os.path.join(path, 'disparity-left', image.replace('.png', '.exr'))
    f = os.path.basename(dispImagePath)
    print 'processing ' + f
    # Usage: <dispTruthLeft> <dispLeft>
    subprocess.call([config['cmd'], dispTruthImagePath, dispImagePath])
    return

# create all the things
for a in config['algorithms']:
    for d in config['datasets']:
        for s in d['sequences']:
            path = os.path.join(d['path'], s)
            mkdirs(os.path.join(path, 'eval', str(a)))
            print 'creating result for sequence: ' + os.path.join(path, 'eval', str(a))
            start = time.time()
            images = getListOfImages(os.path.join(path, 'left')) # only read left directory
            for image in images:
                execute(a, path, image)
            end = time.time()
            delta = "%.2f" % (end - start)
            print delta + ' seconds runtime (sequence: ' + os.path.basename(path) + ')'

exit(0)
