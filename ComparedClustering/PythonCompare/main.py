import argparse

import numpy as np
import pandas as pd

import importlib
import os
from threading import Thread


def execute(alg, repeat, moduledir):
    time, rmses = alg(data, 3813).run(repeat, moduledir)
    print('  Method:', alg.__name__)
    print('  Repeat:', repeat)
    print(' Runtime:', '{:.2f} ± {:.2f}'.format(np.mean(time), np.std(time)))
    print('    RMSE:', '{:.2f} ± {:.2f}'.format(np.mean(rmses), np.std(rmses)))
    print()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('module', help='path of the module to be tested')
    parser.add_argument('input', help='path of the input samples (txt file)')
    parser.add_argument('-d', '--dimension', type=int, default=8,
                        help='dimension of the input data (window size)')
    parser.add_argument('-r', '--repeat', type=int, default=1,
                        help='number of repeats for each method')
    parser.add_argument('-o', '--output', default='output',
                        help='output directory')
    args = parser.parse_args()

    try:
        module = importlib.import_module(args.module)
    except:
        print('Invalid module name.')
        exit()

    try:
        data = np.loadtxt(args.input)
        data = data.reshape(int(len(data)/args.dimension), args.dimension)
    except:
        print('Invalid input file.')
        exit()

    if not (os.path.exists(args.output)):
        os.mkdir(args.output)

    moduledir = "./{}/{}".format(args.output, args.module)

    if not (os.path.exists(moduledir)):
        os.mkdir(moduledir)

    for m in module.methods:
        # t = Thread(target = execute, args = (m, args.repeat, moduledir))
        # t.start()
        execute(m, args.repeat, moduledir)
