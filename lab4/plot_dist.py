import matplotlib.pyplot as plt
import numpy as np

def main():
    bins = 100
    source = {
        'exp' : np.genfromtxt("exp_dist.txt", delimiter="\n"),
        'norm' : np.genfromtxt("norm_dist.txt", delimiter="\n"),
        'poisson' : np.genfromtxt("poisson_dist.txt", delimiter="\n"),
        'weibull' : np.genfromtxt("weibull_dist.txt", delimiter="\n"),
    }
    
    fig, axs = plt.subplots(4, figsize=(20,20))
    
    for i, (k, v) in enumerate(source.items()):
        axs[i].hist(v, bins)
        axs[i].set_title(k)
        
    plt.savefig('hist.png')
        
if __name__ == '__main__':
    main()