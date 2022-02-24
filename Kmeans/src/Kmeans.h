#ifndef _KMEANS_
#define _KMEANS_

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include "Dataset.h"

class Kmeans {
public:
	static void clusteringKmeans(DatasetPtr dataset, std::size_t k, std::size_t iterations);
};

class Evaluation {
public:
	static double evaluation(DatasetPtr clustering_dataset, std::size_t k);
};

#endif // !_KMEANS_

