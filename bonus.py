import matplotlib.pyplot as plt
import pandas as pd
import sklearn
from sklearn import datasets
from sklearn.cluster import KMeans
from sklearn.preprocessing import MinMaxScaler
from sklearn.decomposition import PCA
#%matplotlib inline

# returns a distance between 2 vectors
def distance(vector_1, vector_2):
    distance_between_vectors = 0
    for i in range(len(vector_1)):
        distance_between_vectors += ((vector_1[i] - vector_2[i]) ** 2)
    return distance_between_vectors


def distance_list(args):
    pass


def initialize_clusters(k, list_of_vectors):
    cluster_list = []
    cluster_index_list = [0]
    distance_list = []
    cluster_list.append(list_of_vectors[0])
    for i in range(k):
        for j in range(len(list_of_vectors)):
            if j in cluster_index_list:
                distance_list.append(0)
            else:
                distance_list.append(distance(list_of_vectors[i], list_of_vectors[j]))
        cluster_list.append(list_of_vectors(distance_list.index(max(distance_list))))
    return cluster_list


def distance_from_centroid_to_his_vectors:


def calcInteria(k, data, ):
    sklearn.cluster.KMeans(k, data)
    for i in range(k):
        sum =+ distance_from_centroid_to_his_vectors(cluster, his_vectors)
    return sum

def main():
    iris = datasets.load_iris()
    data = iris.data
    distances_from_centroid = []

    for i in range(9):
        kmeans = KMeans(n_clusters=i+1)
        KModel = kmeans.fit(iris.data)
        distances_from_centroid.append(calcInteria(i+1, data))
    print(calcInteria(5, data))


if __name__ == '__main__':
    main()

#TODO - https://www.youtube.com/watch?v=EItlUEPCIzM - watch video to solve this