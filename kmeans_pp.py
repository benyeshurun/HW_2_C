import sys
import numpy as np
import pandas as pd
import mykmeanssp

# define PY_SSIZE_T_CLEAN
# include <Python.h>

DEFAULT_ITER = 300
MIN_ARGUMENTS = 4


def validate_and_assign_input_user():
    if len(sys.argv) < MIN_ARGUMENTS:
        raise Exception(f"Amount of arguments should be more than 1, amount of arguments={len(sys.argv)}")
    if (not sys.argv[1].isdigit()) or int(sys.argv[1]) < 2:
        raise Exception(f"K input has to be a number and should exceed 0, k={sys.argv[1]}")
    k = int(sys.argv[1])
    max_iter = DEFAULT_ITER
    if len(sys.argv) > 4:
        if (sys.argv[2].isdigit()) and int(sys.argv[2]) > 0:
            max_iter = sys.argv[2]
        else:
            raise Exception(f"max_iter input has to be a number and should exceed 0, max_iter={sys.argv[2]}")
    return k, max_iter


def build_panda():
    try:
        if len(sys.argv) > 4:
            pd_1 = pd.read_csv(sys.argv[3], header=None)
            pd_2 = pd.read_csv(sys.argv[4], header=None)
        else:
            pd_1 = pd.read_csv(sys.argv[2], header=None)
            pd_2 = pd.read_csv(sys.argv[3], header=None)
    except FileNotFoundError:
        print("File not accessible")
    pd_1.rename(columns={list(pd_1)[0]: 'x'}, inplace=True)  # renaming both first columns for the merge
    pd_2.rename(columns={list(pd_2)[0]: 'x'}, inplace=True)
    df = pd.merge(pd_1, pd_2, how='inner', on='x')
    df = df.iloc[:, 1::1]
    return df


# receives the vectors pandas and amount of k clusters and builds a list of clusters
# returns a list of clusters
def choose_random_centrals(vectors, k):
    np.random.seed(0)
    clusters_indexes = np.random.choice(len(vectors.index), k, replace=False)
    print(clusters_indexes)
    clusters_list = []
    for x in clusters_indexes:
        clusters_list.append(vectors.iloc[x,])
    return clusters_list, clusters_indexes


# prints new central after adjusting for the relevant structure
def print_centrals(list_of_clusters):
    for cluster in list_of_clusters:
        for i in range(len(cluster.curr_central)):  # todo - maybe delete this part
            cluster[i] = "{:.4f}".format(cluster[i])
        print(*cluster, sep=' ')


def main():
    k, max_iter = validate_and_assign_input_user()
    list_of_vectors = build_panda()
    amount_of_vectors = len(list_of_vectors)
    list_of_clusters, index_of_clusters = choose_random_centrals(list_of_vectors,k)
    # returns a list of clusters and prints the index of the selected k centroids
    dimensions = len(list_of_vectors.columns)
    list_of_clusters_array = list_of_clusters.to_numpy()
    my_file = np.savetxt('text_for_c.txt', list_of_vectors.values, fmt='%.4f')  # creates text from our vectors pandas
    list_of_final_clusters = mykmeanssp.fit(k, max_iter, my_file, dimensions, amount_of_vectors, index_of_clusters)
    print_centrals(list_of_final_clusters)


if __name__ == '__main__':
    main()
