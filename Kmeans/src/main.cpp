// main.cpp: 定义应用程序的入口点。

#include "Dataset.h"
#include "ReadDataset.h"
#include "Preprocessing.h"
#include "WriteDataset.h"
#include "Kmeans.h"

int main()
{
	std::cout << "Hello CMake." << '\n';

	DatasetPtr origin_dataset = ReadDataset::readxyz(DATA_PATH, 1276507, 3).first;

	//Preprocessing::meanNormalize(clsteringDataset);

	//Kmenas::clusteringKmeans(originDataset, 2, 100);

	//WriteDataset::output_dataset(DATA_PATH, origin_dataset);
	// std::cout << dataset->nrows; // total rows: 1276507	
	

	return 0;

}

