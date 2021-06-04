import inline as inline
import matplotlib
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sklearn
from sklearn import datasets
from sklearn.decomposition import PCA
#%matplotlib inline

def calcInteria(k, data, ):
    sklearn.cluster.KMeans(k, data)

def main():
    # import some data to play with
    iris = datasets.load_iris()
    data = iris.data[:, :4]
    print(calcInteria(5, data))


if __name__ == '__main__':
    main()
