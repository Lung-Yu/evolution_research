import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from sklearn import datasets
from sklearn.decomposition import PCA
import numpy as np


# import some data to play with
iris = datasets.load_iris()

def make_iris_files(train_set_rate):
    #init vals
    total_size = len(iris.data)
    train_set_size = total_size * train_set_rate
    idxs = np.arange(total_size)
    np.random.shuffle(idxs)
    #open file
    train_file = 'train_iris.txt'
    test_file = 'test_iris.txt'
    mod = 'w+'
    f_train = open(train_file, mod)
    f_test = open(test_file, mod)

    #write dats
    for i in range(0,total_size):
        idx = idxs[i]
        item = iris.data[idx]
        item_str = ''
        for val in item:
            item_str += str(val) + ' '
        item_str += str(iris.target[idx])

        if i > train_set_size :
            f_test.write(item_str+'\n')
        else:
            f_train.write(item_str+'\n')
    f_train.close()
    f_test.close()

make_iris_files(0.7)
