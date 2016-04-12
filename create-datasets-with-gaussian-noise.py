#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing

config = {
    'cmd': '/Users/bjohn/git/thesis/disparity-evaluation/5_ImageDiminisher/bin/ImageDiminisher',
    'sigma': [5],
    'datasets': [
        {
           'name': 'cambridge',
           'path': '/Users/bjohn/Desktop/datasets/',
           'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
    ]
}

def getListOfImages(path):
    images = []
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f)): images.append(os.path.splitext(f)[0])
    images.sort()
    return images

# wrap image diminisher
def execute(path, sigma):
    subprocess.call([config['cmd'], path, path, str(sigma)])
    return

# create all the things
for d in config['datasets']:
    for sigma in config['sigma']:
        for s in d['sequences']:
            path = os.path.join(d['path'], d['name'] + '-gn-' + str(sigma), s)
            pathLeft = os.path.join(path, 'left')
            pathRight = os.path.join(path, 'right')
            for image in getListOfImages(pathLeft):
                pathInForLeft = os.path.join(pathLeft, image + '.png')
                pathInForRight = os.path.join(pathRight, image + '.png')
                print pathInForLeft
                print pathInForRight
                #execute(pathInForLeft, sigma)
                #execute(pathInForRight, sigma)

exit(0)
