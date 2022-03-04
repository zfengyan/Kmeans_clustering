// main.cpp: 定义应用程序的入口点。

#include "Dataset.h"
#include "ReadDataset.h"
#include "Preprocessing.h"
#include "WriteDataset.h"
#include "Kmeans.h"

int main()
{
	/*
	* ***************************************************************************
	* adjust the parameters here
	*/
	
	std::size_t k = 5; // k - means, k clusters
	std::size_t iterations = 100; // iterations for k - means

	/****************************************************************************/

	
	std::ios::sync_with_stdio(false); // speed up for std::cin and std::cout


	std::cout << "Hello CMake." << '\n';


	std::pair<DatasetPtr, DatasetPtr> dataset_pair;


	std::size_t numFeaturePoints(0); // store the number of feature points
	dataset_pair = ReadDataset::readxyz(DATA_PATH, numFeaturePoints);
	

	DatasetPtr origin_dataset = dataset_pair.first;
	DatasetPtr clustering_dataset = dataset_pair.second;


	Preprocessing::meanNormalize(clustering_dataset);	
	Kmeans::clusteringKmeans(clustering_dataset, k, iterations); // clusters, iteration
	

	std::vector<double> accuracy; // accuracy for each cluster group
	accuracy.reserve(k);

	std::vector<std::size_t> correctNums; // numbers for correct results
	correctNums.reserve(numFeaturePoints); // maximum numbers: numFeaturePoints(numFeaturePoints feature points in total)

	std::pair<std::vector<double>, std::vector<std::size_t>> result_pair;

	Evaluation::evaluation(clustering_dataset, k, result_pair);


	/*
	* calculate the overall statistics
	*/

	// calculate the avg accuracy
	double accuracy_N = (double)result_pair.first.size(); // result_pair.first: accuracy vector
	double accumulate(0);
	double accumulated_accuracy(0);
	for (auto& a : result_pair.first)
	{
		accumulate += a;
	}
	accumulated_accuracy = accumulate / accuracy_N;

	// calculate the numbers of correct records and wrong records
	std::size_t correctResultNum(0);
	for (auto& recordNum : result_pair.second)
	{
		correctResultNum += recordNum;
	}
	std::size_t wrongResultNum = numFeaturePoints - correctResultNum;

	// calculate the overall accuracy
	double overall_accuracy = (double)correctResultNum / numFeaturePoints;

	// print the overall statistics
	std::cout << "------------------------" << " " << "Overall statistics: "
		<< " " << " " << "------------------------" << '\n';

	std::cout << "Numbers: " << '\n';
	std::cout << "Correct numbers: " << correctResultNum << " " << "out of " << numFeaturePoints << '\n';
	std::cout << "Wrong numbers: " << wrongResultNum << " " << "out of " << numFeaturePoints << '\n';
	std::cout << '\n';
	std::cout << "Accuracy: " << '\n';
	std::cout << "Unweighted avg accuracy = " << accumulated_accuracy << '\n';
	std::cout << "Overall accuracy = " << overall_accuracy << '\n';

	// write messages to the report
	std::string filepath = DATA_PATH;
	std::string filesuffix = "/report.txt";
	std::string filename = filepath + filesuffix;

	std::ofstream myfile;
	myfile.open(filename, std::ios::app); // app: append

	myfile << "------------------------" << " " << "Overall statistics: "
		<< " " << " " << "------------------------" << '\n';

	myfile << "Numbers: " << '\n';
	myfile << "Correct numbers: " << correctResultNum << " " << "out of " << numFeaturePoints << '\n';
	myfile << "Wrong numbers: " << wrongResultNum << " " << "out of " << numFeaturePoints << '\n';
	myfile << '\n';
	myfile << "Accuracy: " << '\n';
	myfile << "Unweighted avg accuracy = " << accumulated_accuracy << '\n';
	myfile << "Overall accuracy = " << overall_accuracy << '\n';

	myfile.close();
	// write messages

	// WriteDataset::output_dataset(DATA_PATH, clustering_dataset); // call WriteDataset function
	// after evaluation() 

	return 0;

}

