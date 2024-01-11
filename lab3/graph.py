import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def make_plots(src, name):
    plt.hist(src, bins=50)
    plt.savefig('imgs/hist_{}.jpg'.format(name))
    plt.clf()

    if name == '1000':
        plt.scatter(range(len(src)), src, s=10)
        plt.savefig('imgs/scatter_{}.jpg'.format(name))
        plt.clf()

        pd.plotting.lag_plot(pd.Series(src), marker='.')
        plt.savefig('imgs/lag_{}.jpg'.format(name))
        plt.clf()

        pd.plotting.autocorrelation_plot(pd.Series(src), linewidth=0.5)
        plt.ylim(-0.15,0.15)
        plt.savefig('imgs/autocorr_{}.jpg'.format(name))
        plt.clf()


def read_file(src):
    with open(src) as f:
        values = f.readlines()

    return np.array([float(x.rstrip()) for x in values])

def main():
    xst64 = {
        '1000': read_file('xst64_1000.txt'),
        '10000': read_file('xst64_10000.txt'),
        '100000': read_file('xst64_100000.txt'),
        '1000000': read_file('xst64_1000000.txt'),
        '10000000': read_file('xst64_10000000.txt')
    }

    for k, v in xst64.items():
        make_plots(v, k)

if __name__ == '__main__':
    main()