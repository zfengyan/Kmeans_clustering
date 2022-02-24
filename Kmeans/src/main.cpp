// main.cpp: 定义应用程序的入口点。

#include "Dataset.h"
#include "ReadDataset.h"
#include "Preprocessing.h"
#include "WriteDataset.h"
#include "Kmeans.h"

int main()
{
	std::ios::sync_with_stdio(false); // speed up for std::cin and std::cout

	std::cout << "Hello CMake." << '\n';

	std::pair<DatasetPtr, DatasetPtr> dataset_pair;
	dataset_pair = ReadDataset::readxyz(DATA_PATH, 1276507, 3, 500, 3);

	DatasetPtr origin_dataset = dataset_pair.first;
	DatasetPtr clustering_dataset = dataset_pair.second;


	Preprocessing::meanNormalize(clustering_dataset);

	//Kmenas::clusteringKmeans(originDataset, 2, 100);

	WriteDataset::output_dataset(DATA_PATH, clustering_dataset);
	// std::cout << dataset->nrows; // total rows: 1276507	
	

	return 0;

}

