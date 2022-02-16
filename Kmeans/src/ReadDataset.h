#ifndef _READ_DATASET_
#define _READ_DATASET_

// ================================ print message
#include <iostream>

// ================================ xyz file reader, laz file writer
#include <lasreader.hpp>
#include <laswriter.hpp>

// ================================ read all format files in one folder
#include <io.h>
#include <string>
#include <vector>

#include "Dataset.h"

class ReadDataset {
private:
	static void GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format);
public:
	static DatasetPtr readxyz(const std::string& datapath);
};


#endif // !_READ_DATASET_

