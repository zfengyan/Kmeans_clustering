#ifndef _OUTPUT_DATASET_
#define _OUTPUT_DATASET_

#include "ReadDataset.h"

class WriteDataset {
public:
	/*
	* write original ground truth point cloud to the file
	*/
	static void output_dataset(const std::string& filename, DatasetPtr dataset);
};


#endif // !_OUTPUT_DATASET_

