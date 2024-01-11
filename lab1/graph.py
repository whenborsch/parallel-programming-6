import matplotlib.pyplot as plt
import numpy as np

def read_file(src):
    with open(src) as f:
        values = f.readlines()
    
    return np.array([int(x.rstrip()) for x in values])

def main():
    binomial = read_file('binomial.txt')
    plt.hist(binomial, bins=500)    
    plt.savefig('binomial_clt.jpg')

if __name__ == '__main__':
    main()