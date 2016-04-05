#!/usr/bin/env python

from os import listdir, makedirs, getcwd
from os.path import isfile, join, exists, splitext
from shutil import move

datasets = ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
workDir = getcwd()
copyPattern = [{'L0': 'left'}, {'R0': 'right'}, {'TL0': 'disparity-left'}, {'TR0': 'disparity-right'}]

def mkdirs(path):
    if not exists(path): makedirs(path)
    return

def getImages(path):
    ret = []
    for f in listdir(path):
        if isfile(join(dDir, f)) and splitext(f)[1] == '.png':
            ret.append(f)
    return ret

for d in datasets:
    dDir = join(workDir, d)
    images = getImages(dDir)
    for p in copyPattern:
        k = p.keys()[0]
        v = p[k]
        mkdirs(join(dDir, v))
        for f in images:
            if splitext(f)[0][:len(k)] == k:
                filename = 'image' + splitext(f)[0][len(k)-1:] + '.png'
                move(join(dDir, f), join(dDir, v, filename))

exit(0)

