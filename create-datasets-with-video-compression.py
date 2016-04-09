#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing

config = {
    'crf': [0, 14, 28, 40, 51],
    'datasets': [
        {
            'name': 'cambridge',
            'path': '/Users/bjohn/desktop/datasets/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        }
    ]
}

# wrap ffmpeg commands
def execute(crf):
    # imagesToVideo
    subprocess.call(['ffmpeg', '-loglevel', 'panic', '-f', 'image2', '-i', 'image%04d.png', 'video.mpg'])
    # applyCompression
    subprocess.call(['ffmpeg', '-loglevel', 'panic', '-i', 'video.mpg', '-c:v', 'libx265', '-x265-params', 'crf=' + str(crf), '-c:a', 'copy', 'video.mkv'])
    # videoToImages
    subprocess.call(['ffmpeg', '-loglevel', 'panic', '-i', 'video.mkv', 'image%04d.png'])
    # clean-up
    os.remove('video.mpg')
    os.remove('video.mkv')
    return

# create all the things
for d in config['datasets']:
    for c in config['crf']:
        for s in d['sequences']:
            path = os.path.join(d['path'], d['name'] + '-vc-' + str(c), s)
            pathLeft = os.path.join(path, 'left')
            pathRight = os.path.join(path, 'right')
            os.chdir(pathLeft)
            execute(c)
            os.chdir(pathRight)
            execute(c)
            print path

exit(0)
