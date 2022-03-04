#ifndef _OUTPUT_DATASET_
#define _OUTPUT_DATASET_

#include "ReadDataset.h"

class WriteDataset {
public:
	/*
	* write original ground truth point cloud to the file
	*/
	static void output_origin_dataset(const std::string& filepath, DatasetPtr dataset);


	/*
	* set the cluster results of origin_dataset
	* according to the result of clustering_dataset
	* linked by: fileid -- indicating the point belongs to which file
	*/
	static void assign_result_dataset(
		DatasetPtr origin_dataset,
		DatasetPtr clustering_dataset);


	/*
	* write classification result of the original point cloud to the file
	* NB: this function needs to be called after the assign_result_dataset() function
	*/
	static void output_result_dataset(const std::string& filepath, DatasetPtr dataset);
};


#endif // !_OUTPUT_DATASET_

