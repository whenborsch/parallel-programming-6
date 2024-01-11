import matplotlib.pyplot as plt
import pandas

if __name__ == "__main__":
    result = pandas.read_csv('data.csv')

    x = result['x']
    y = result['y']
    in_p = result['mask'] == 1
    out_p = result['mask'] == 0

    plt.scatter(x[in_p], y[in_p], color='red', marker='.')
    plt.scatter(x[out_p], y[out_p], marker='.')
    plt.savefig('res.jpg')
