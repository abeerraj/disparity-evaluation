#!/usr/bin/env python

import os
from shutil import copyfile

config = {
    'datasets': [
        {
            'path': '/Users/bjohn/Desktop/datasets/cambridge/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
        {
            'path': '/Users/bjohn/Desktop/datasets/cambridge-gn-5/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
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
        # {
        #    'path': '/Users/bjohn/Desktop/datasets/tsukuba/',
        #    'sequences': ['01-tsukuba']
        # },
    ]
}

def getListOfImages(path):
    images = []
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f)): images.append(os.path.splitext(f)[0])
    images.sort()
    return images

# create all the things
for d in config['datasets']:
    for s in d['sequences']:
        path = os.path.join(d['path'], s)
        images = getListOfImages(os.path.join(path, 'left')) # only read left directory
        masksPath = os.path.join(path, 'masks')
        prev = ''
        for i in images:
            f = os.path.join(masksPath, i + '-mask-salient.png')
            if not os.path.isfile(f):
                print 'missing mask, replace with previous'
                print prev
                print f
                copyfile(prev, f)
            prev = f

exit(0)
