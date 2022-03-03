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

	std::size_t attribute_columns = 2; // number of attribute columns
	dataset_pair = ReadDataset::readxyz(DATA_PATH, 1276507, 3, 500, attribute_columns); // 500 rows

	DatasetPtr origin_dataset = dataset_pair.first;
	DatasetPtr clustering_dataset = dataset_pair.second;

	Preprocessing::meanNormalize(clustering_dataset);

	std::size_t k = 3; // k - means, k clusters
	Kmeans::clusteringKmeans(clustering_dataset, k, 100); // clusters, iteration
	
	std::vector<double> accuracy; // accuracy for each cluster group
	accuracy.reserve(k);
	Evaluation::evaluation(clustering_dataset, k, accuracy); // k=3

	/*
	* calculate the accuracy
	*/
	double accuracy_N = (double)accuracy.size();
	double accumulate(0);
	double accumulated_accuracy(0);
	for (auto& a : accuracy)
	{
		accumulate += a;
	}
	accumulated_accuracy = accumulate / accuracy_N;

	std::cout << "------------------------" << " " << "Overall accuracy: "
		<< " " << " " << "------------------------" << '\n';
	std::cout << "Accuracy = " << accumulated_accuracy << '\n';

	// WriteDataset::output_dataset(DATA_PATH, clustering_dataset); // call WriteDataset function
	// after evaluation() 

	return 0;

}

