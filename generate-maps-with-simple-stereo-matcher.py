#!/usr/bin/env python

import sys
import os
import subprocess
import time

config = {
    'algorithms': [10, 11, 12],
    'cmd': '/Users/bjohn/git/thesis/disparity-evaluation/4_NaiveImplementation/bin/NaiveImplementation',
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
    ]
}

def mkdirs(path):
    if not os.path.exists(path): os.makedirs(path)
    return

def lastOfImages(path):
    images = []
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f)):
            if not f.startswith('.'):
                images.append(f)
    images.sort(reverse=True)
    return images[0]

def execute(left, right, out, count, temporal, weighted, last):
    # Usage: <left> <right> <out> <count> <temporal> <weighted>
    f = os.path.basename(last)
    if os.path.isfile(last):
        print 'skipping ' + out
        return
    print 'processing ' + out
    subprocess.call([config['cmd'], left, right, out, str(count), str(temporal), str(weighted)])
    return

# create all the things
for a in config['algorithms']:
    for d in config['datasets']:
        for s in d['sequences']:
            path = os.path.join(d['path'], s)
            left = os.path.join(d['path'], s, 'left')
            right = os.path.join(d['path'], s, 'right')
            out = os.path.join(d['path'], s, 'computed', str(a))
            mkdirs(out)
            last = lastOfImages(os.path.join(path, 'left'))
            pure = int(last.replace('.png', '').replace('image', ''))
            lastPath = os.path.join(out, last);
            temporal = 1 if a > 10 else 0
            weighted = 1 if a > 11 else 0
            execute(left, right, out, pure, temporal, weighted, last)

exit(0)
