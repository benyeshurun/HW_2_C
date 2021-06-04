#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SQ(x) ((x)*(x))
#define K_ARGUMENT 2
#define MAX_ITER_ARGUMENT 3
#define DEFAULT_NUM_OF_ITERS 300
#define MAX_CHAR_AT_ONE_LINE 1000
#define COMMA_CHAR ','
#define END_OF_STRING '\0'

typedef struct {
    double *prevCentroid;
    double *currCentroid;
    int counter; /* Number of vectors (data points) in cluster */
} Cluster;

void calcDimAndNumOfVectors(int *dimension, int *numOfVectors); /* Calculate vectors dimension and number of total vectors */
double **initVectorsArray(const int *numOfVectors, const int *dimension, char* myFile); /* Insert vectors into an array */
Cluster *initClusters(double **vectorsArray, const int *k, const int *dimension, int listOfIndex[]); /* Initialize empty clusters array */
double vectorsNorm(const double *vec1, const double *vec2, const int *dimension); /* Calculate the norm between 2 vectors */
int findMyCluster(double *vec, Cluster *clustersArray, const int *k, const int *dimension); /* Return the vector's closest cluster (in terms of norm) */
void assignVectorsToClusters(double **vectorsArray, Cluster *clustersArray, const int *k, const int *numOfVectors, const int *dimension); /* For any vector assign to his closest cluster */
int recalcCentroids(Cluster *clustersArray, const int *k, const int *dimension); /* Recalculate clusters' centroids, return number of changes */
void initCurrCentroidAndCounter(Cluster *clustersArray, const int *k, const int *dimension); /* Set curr centroid to prev centroid and reset the counter */
void printFinalCentroids(Cluster *clustersArray, const int *k, const int *dimension); /* Print clusters' final centroids */
void freeMemoryVectorsClusters(double **vectorsArray, Cluster *clustersArray, const int *k); /* Free the allocated memory */
void validateAndAssignInput(int argc, char **argv, int *maxIter, int *k); /* Validate and assign k and max_iter input */

static int fit(int k, int maxIter, char* myFile, int dimension, int numOfVectors, int kIndexes[]) {
    int i, changes;
    double **vectorsArray;
    Cluster *clustersArray;

    //validateAndAssignInput(argc, argv, &maxIter, &k);
    if (k >= numOfVectors) { /* Number of clusters can't be more than the number of vectors */
        printf("Number fo clusters (%d) can't be more than the number of data points (%d).\n", k, numOfVectors);
        return 0;
    }
    /* Initialize vectors (data points) and clusters arrays */
    vectorsArray = initVectorsArray(&numOfVectors, &dimension, myFile);
    clustersArray = initClusters(vectorsArray, &k, &dimension, kIndexes);

    for (i = 0; i < maxIter; ++i) {
        initCurrCentroidAndCounter(clustersArray, &k, &dimension); /* Update curr centroid to prev centroid and reset the counter */
        assignVectorsToClusters(vectorsArray, clustersArray, &k, &numOfVectors, &dimension);
        changes = recalcCentroids(clustersArray, &k, &dimension); /* Calculate new centroids */
        if (changes == 0) { /* Centroids stay unchanged in the current iteration */
            break;
        }
    }

    printFinalCentroids(clustersArray, &k, &dimension);
    freeMemoryVectorsClusters(vectorsArray, clustersArray, &k);
    return 0;
}


/*
 * This actually defines the fit function using a wrapper C API function
 * The wrapping function needs a PyObject* self argument.
 * This is a requirement for all functions and methods in the C API.
 * It has input PyObject *args from Python.
 */
static PyObject* fit_connect(PyObject *self, PyObject *args)
{
    PyObject *list
    char* myFile ;
    int k, dimension, numOfVectors, maxIter, *kIndexes;
    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if(!PyArg_ParseTuple(args, "iisiiO!", &k, &maxIter, &myFile, &dimension, &numOfVectors, &PyList_Type, &list))
        return NULL;
    kIndexes = (int *) malloc ((*k)*sizeof (int));
    assert(indexes != NULL);
    for (Py_ssize_t i = 0; i < k; i++) {
        indexes[i] = PyList_GetItem(list, i);

    return Py_BuildValue(fit(k, maxIter, myFile, dimension, numOfVectors, kIndexes)); /*  prints the new centroids  */
}


/*
 * This array tells Python what methods this module has.
 * We will use it in the next structure
 */
static PyMethodDef _method[] = {
        {"fit",                      /* the Python method name that will be used */
                (PyCFunction) fit_c, /* the C-function that implements the Python function and returns static PyObject*  */
                    METH_VARARGS,   /* flags indicating parametersaccepted for this function */
                   NULL},      /*  The docstring for the function (PyDoc_STR("")) */
        {NULL, NULL, 0, NULL}        /* The is a sentinel. Python looks for this entry to know that all
                                       of the functions for the module have been defined. */
};

/*

/* This initiates the module using the above definitions. */
static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp", /* name of module */
        NULL, /* module documentation, may be NULL */
        -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
        _method /* the PyMethodDef array from before containing the methods of the extension */
};

/*
 * The PyModuleDef structure, in turn, must be passed to the interpreter in the module’s initialization function.
 * The initialization function must be named PyInit_name(), where name is the name of the module and should match
 * what we wrote in struct PyModuleDef.
 * This should be the only non-static item defined in the module file
 */
PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
    return PyModule_Create(&moduledef);
}


void calcDimAndNumOfVectors(int *dimension, int *numOfVectors) {
    char line[MAX_CHAR_AT_ONE_LINE];
    char *c;
    *dimension = 1;
    *numOfVectors = 1;

    scanf("%s", line);
    for (c = line; *c != '\0'; c++) {
        *dimension += *c == COMMA_CHAR ? 1 : 0; /* Calc vectors' dimension */
    }

    while (scanf("%s", line) != EOF) {
        (*numOfVectors)++; /* Calc number of vectors (datapoints) */
    }
    rewind(stdin); /* Move back to the beginning of the input file */
}

double **initVectorsArray(const int *numOfVectors, const int *dimension, char* myFile) {
    int i, j;
    char ch;
    double *matrix, **vectorsArray;
    File *fPtr;
    /* Allocate memory for vectorsArray */
    matrix = (double *) malloc((*numOfVectors) * ((*dimension) + 1) * sizeof(double));
    assert(matrix != NULL);
    vectorsArray = malloc((*numOfVectors) * sizeof(double *));
    assert(vectorsArray != NULL);
    fPtr = fopen(myFile,"r");
    for (i = 0; i < *numOfVectors; ++i) {
        vectorsArray[i] = matrix + i * ((*dimension) + 1); /* Set VectorsArray to point to 2nd dimension array */
        for (j = 0; j < *dimension; ++j) {
            scanf(fPtr ,"%lf%c", &vectorsArray[i][j]);
        }
    }
    fclose(fPtr);
    return vectorsArray;
}

Cluster *initClusters(double **vectorsArray, const int *k, const int *dimension, int listOfIndex[]) {
    int i, j;
    Cluster *clustersArray;
    /* Allocate memory for clustersArray */
    clustersArray = (Cluster *) malloc((*k) * sizeof(Cluster));
    assert(clustersArray != NULL);

    for (i = 0; i < *k; ++i) {
        clustersArray[i].prevCentroid = (double *) malloc((*dimension) * sizeof(double));
        clustersArray[i].currCentroid = (double *) malloc((*dimension) * sizeof(double));
        assert(clustersArray[i].prevCentroid != NULL && clustersArray[i].currCentroid != NULL);
        clustersArray[i].counter = 0;

        for (j = 0; j < *dimension; ++j) {
            clustersArray[i].currCentroid[j] = vectorsArray[listOfIndex[i]][j]; /* Assign the k vectors from the list of indexes to their corresponding clusters */
        }
    }
    return clustersArray;
}

double vectorsNorm(const double *vec1, const double *vec2, const int *dimension) {
    double norm = 0;
    int i;
    for (i = 0; i < *dimension; ++i) {
        norm += SQ(vec1[i] - vec2[i]);
    }
    return norm;
}

int findMyCluster(double *vec, Cluster *clustersArray, const int *k, const int *dimension) {
    int myCluster, j;
    double minNorm, norm;

    myCluster = 0;
    minNorm = vectorsNorm(vec, clustersArray[0].prevCentroid, dimension);
    for (j = 1; j < *k; ++j) { /* Find the min norm == closest cluster */
        norm = vectorsNorm(vec, clustersArray[j].prevCentroid, dimension);
        if (norm < minNorm) {
            myCluster = j;
            minNorm = norm;
        }
    }
    return myCluster;
}

void assignVectorsToClusters(double **vectorsArray, Cluster *clustersArray, const int *k, const int *numOfVectors, const int *dimension) {
    int j, i, myCluster;
    double *vec;
    for (j = 0; j < *numOfVectors; ++j) {
        vec = vectorsArray[j];
        myCluster = findMyCluster(vectorsArray[j], clustersArray, k, dimension);
        vec[*dimension] = myCluster; /* Set vector's cluster to his closest */
        for (i = 0; i < *dimension; ++i) {
            clustersArray[myCluster].currCentroid[i] += vec[i]; /* Summation of the vectors Components */
        }
        clustersArray[myCluster].counter++; /* Count the number of vectors for each cluster */
    }
}

int recalcCentroids(Cluster *clustersArray, const int *k, const int *dimension) {
    int i, j, changes = 0;
    Cluster cluster;
    for (i = 0; i < *k; ++i) {
        cluster = clustersArray[i];
        for (j = 0; j < *dimension; ++j) {
            cluster.currCentroid[j] /= cluster.counter; /* Calc the mean value */
            changes += cluster.prevCentroid[j] != cluster.currCentroid[j] ? 1 : 0; /* Count the number of changed centroids' components */
        }
    }
    return changes;
}

void initCurrCentroidAndCounter(Cluster *clustersArray, const int *k, const int *dimension) {
    int i, j;
    for (i = 0; i < *k; ++i) {
        for (j = 0; j < *dimension; ++j) {
            clustersArray[i].prevCentroid[j] = clustersArray[i].currCentroid[j]; /* Set prev centroid to curr centroid */
            clustersArray[i].currCentroid[j] = 0; /* Reset curr centroid */
        }
        clustersArray[i].counter = 0; /* Reset counter */
    }
}

void printFinalCentroids(Cluster *clustersArray, const int *k, const int *dimension) {
    int i, j;
    for (i = 0; i < *k; ++i) {
        for (j = 0; j < *dimension; ++j) {
            if (j > 0)
                printf("%c", COMMA_CHAR);
            printf("%0.4f", clustersArray[i].currCentroid[j]); /* Print with an accuracy of 4 digits after the dot */
        }
        printf("\n");
    }
}

void freeMemoryVectorsClusters(double **vectorsArray, Cluster *clustersArray, const int *k) {
    int i;
    /* Free clusters */
    for (i = 0; i < *k; ++i) {
        free(clustersArray[i].currCentroid);
        free(clustersArray[i].prevCentroid);
    }
    free(clustersArray);

    /* Free vectors */
    free(*vectorsArray);
    free(vectorsArray);
}

void validateAndAssignInput(int argc, char **argv, int *maxIter, int *k) {
    char *nextCh;
    if (argc < K_ARGUMENT) {
        printf("The program needs at least one argument.\n");
        exit(0);
    }
    *k = strtol(argv[K_ARGUMENT - 1], &nextCh, 10);
    if (*k < 1 || *nextCh != END_OF_STRING) { /* Contain not only digits or integer less than 1 */
        printf("K argument must be an integer number greater than 0.\n");
        exit(0);
    }
    if (argc >= MAX_ITER_ARGUMENT) {
        *maxIter = strtol(argv[MAX_ITER_ARGUMENT - 1], &nextCh, 10);
        if (*maxIter < 1 || *nextCh != END_OF_STRING) { /* Contain not only digits or integer less than 1 */
            printf("Max iteration argument must be an integer number greater than 0.\n");
            exit(0);
        }
    } else {
        *maxIter = DEFAULT_NUM_OF_ITERS;
    }
}


