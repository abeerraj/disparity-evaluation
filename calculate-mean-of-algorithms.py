#!/usr/bin/env python

import sys
import os
import subprocess
import time
import multiprocessing
import csv

config = {
    'algorithms': range(13),
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
            'path': '/Users/bjohn/desktop/datasets/cambridge-gn-10/',
            'sequences': ['01-book', '02-street', '03-tanks', '04-temple', '05-tunnel']
        },
        {
            'path': '/Users/bjohn/desktop/datasets/cambridge-gn-15/',
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
        {
           'path': '/Users/bjohn/Desktop/datasets/tsukuba/',
           'sequences': ['01-tsukuba']
        },
    ]
}

def meanOfResults(path, a):
    results = []
    f = os.path.join(path, 'aggregated_results.csv')
    print 'processing file: ' + f

    result = a + ';'
    for col in range(1,21):
        with open(f) as csvfile:
            reader = csv.reader(csvfile, delimiter=';')
            next(reader) # exclude header
            values = [float(row[col]) for row in reader]
            average = sum(values) / len(values)
            result += str(average) + ';'
    return result + '\n'

def meanOfRuntime(path, a):
    results = []
    f = os.path.join(path, 'aggregated_runtime.csv')
    print 'processing file: ' + f
    result = a + ';'
    with open(f) as csvfile:
        reader = csv.reader(csvfile, delimiter=';')
        next(reader) # exclude header
        if int(a) > 9:
            values = [float(row[0]) for row in reader]
        else:
            values = [float(row[1]) for row in reader]
        average = sum(values) / len(values)
        result += str(average) + ';'
    return result + '\n'

# aggregate all the things

for d in config['datasets']:
    for s in d['sequences']:
        mean_results = []
        mean_runtime = []
        mean_results.append('algorithm;rmseAll;rmseDisc;rmseNoc;rmseTex;rmseSal;pbmpAll1;pbmpDisc1;pbmpNoc1;pbmpTex1;pbmpSal1;pbmpAll2;pbmpDisc2;pbmpNoc2;pbmpTex2;pbmpSal2;pbmpAll4;pbmpDisc4;pbmpNoc4;pbmpTex4;pbmpSal4;\n')
        mean_runtime.append('algorithm;runtime;\n')

        pathAggregatedMean = os.path.join(d['path'], s, 'eval')

        for a in config['algorithms']:
                pathEval = os.path.join(d['path'], s, 'eval', str(a))
                mean_results.append(meanOfResults(pathEval, str(a)))
                mean_runtime.append(meanOfRuntime(pathEval, str(a)))

        with open(os.path.join(pathAggregatedMean, 'mean_results.csv'), 'w+') as f:
            for item in mean_results:
                  f.write("%s" % item)

        with open(os.path.join(pathAggregatedMean, 'mean_runtime.csv'), 'w+') as f:
            for item in mean_runtime:
                  f.write("%s" % item)

exit(0)
