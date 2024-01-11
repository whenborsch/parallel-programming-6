import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def make_plots(src, name):
    if name == 'poisson':
        plt.hist(src, bins=40, density=True)
    else:
        plt.hist(src, bins=500, density=True)
    plt.savefig('imgs/{}.jpg'.format(name))
    plt.clf()

def read_file(src):
    with open(src) as f:
        values = f.readlines()

    return np.array([float(x.rstrip()) for x in values])

def main():
    xst64 = {
        'norm': read_file('norm.txt'),
        'exp': read_file('exp.txt'),
        'poisson': read_file('poisson.txt'),
        'weibull': read_file('weibull.txt'),
    }

    for k, v in xst64.items():
        make_plots(v, k)

if __name__ == '__main__':
    main()