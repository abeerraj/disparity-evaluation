#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing

config = {
    'algorithms': range(6),
    'datasets': [
        {
            'path': '/Users/bjohn/Desktop/datasets/svddd/',
            'sequences': ['02-rabbit', '02-rabbit-neg']
        },
        # {
        #     'path': '/Users/bjohn/Desktop/datasets/cambridge/',
        #     'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        # },
        # {
        #     'path': '/Users/bjohn/Desktop/datasets/cambridge-gn-5/',
        #     'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        # },
        # {
        #     'path': '/Users/bjohn/desktop/datasets/cambridge-gn-10/',
        #     'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        # },
        # {
        #     'path': '/Users/bjohn/desktop/datasets/cambridge-gn-15/',
        #     'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        # },
        # {
        #     'path': '/Users/bjohn/Desktop/datasets/cambridge-vc-14/',
        #     'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        # },
        # {
        #     'path': '/Users/bjohn/Desktop/datasets/cambridge-vc-28/',
        #     'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        # },
        # {
        #     'path': '/Users/bjohn/Desktop/datasets/cambridge-vc-40/',
        #     'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        # },
        # {
        #    'path': '/Users/bjohn/Desktop/datasets/tsukuba/',
        #    'sequences': ['01-tsukuba']
        # },
    ]
}

def aggregateResults(path, i):
    results = []
    results.append('prefix;rmseAll;rmseDisc;rmseNoc;rmseTex;rmseSal;pbmpAll1;pbmpDisc1;pbmpNoc1;pbmpTex1;pbmpSal1;pbmpAll2;pbmpDisc2;pbmpNoc2;pbmpTex2;pbmpSal2;pbmpAll4;pbmpDisc4;pbmpNoc4;pbmpTex4;pbmpSal4;\n')
    for result in i:
        f = os.path.join(path, result + '_result.txt')
        print 'processing file: ' + f
        with open(f) as r:
            content = r.readlines()[1]
            results.append(content)
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
            pathResults = os.path.join(d['path'], s, 'eval', str(a))
            pathImages = os.path.join(d['path'], s, 'left')
            i = getListOfImages(pathImages)
            outcome = aggregateResults(pathResults, i)
            with open(os.path.join(pathResults, 'aggregated_results.csv'), 'w+') as f:
                for item in outcome:
                      f.write("%s" % item)

exit(0)
