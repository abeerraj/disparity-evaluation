#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing

config = {
    'algorithms': range(13),
    'datasets': [
        {
            'path': '/Users/bjohn/desktop/datasets/cambridge/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
        #{
        #    'path': '/Users/bjohn/desktop/datasets/tsukuba/',
        #    'sequences': ['01-tsukuba']
        #}
    ]
}

def aggregateResults(a, path, i):
    results = []
    results.append('prefix;runtime;\n')
    if a > 9:
        f = os.path.join(path, 'runtime.txt')
        print 'processing file: ' + f
        with open(f) as r:
            content = r.readlines()[0]
            results.append(result + ';' + content + ";\n")
    else:
        for result in i:
            f = os.path.join(path, result + '_runtime.txt')
            print 'processing file: ' + f
            with open(f) as r:
                content = r.readlines()[0]
                results.append(result + ';' + content + ";\n")
    return results

def getListOfImages(path):
    images = []
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f)): images.append(os.path.splitext(f)[0])
    images.sort()
    return images

# aggregate all the things
for a in config['algorithms']:
    for d in config['datasets']:
        for s in d['sequences']:
            pathResults = os.path.join(d['path'], s, 'computed', str(a))
            pathEval = os.path.join(d['path'], s, 'eval', str(a))
            pathImages = os.path.join(d['path'], s, 'left')
            i = getListOfImages(pathImages)
            outcome = aggregateResults(a, pathResults, i)
            with open(os.path.join(pathEval, 'aggregated_runtime.csv'), 'w+') as f:
                for item in outcome:
                      f.write("%s" % item)

exit(0)
