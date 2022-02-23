#ifndef _KMEANS_
#define _KMEANS_

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include "Dataset.h"

class Kmenas {
public:
	static void clusteringKmeans(DatasetPtr dataset, std::size_t k, std::size_t iterations);
};

#endif // !_KMEANS_

