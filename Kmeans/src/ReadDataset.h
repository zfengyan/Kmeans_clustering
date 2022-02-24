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
#include <utility>

#include "Dataset.h"

struct Point{ // used for storing the points for each pointcloud file
	double x;
	double y;
	double z;

	Point(double px, double py, double pz):
		x(px), y(py), z(pz){}
};


class ReadDataset {
private:
	static void GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format);
public:

	/*
	* read all xyz files and return a pointer
	*/
	static std::pair<DatasetPtr, DatasetPtr> readxyz(
		const std::string& datapath,
		std::size_t p_nrows,
		std::size_t p_ncols,
		std::size_t p_num_rows,
		std::size_t p_num_attributes);

};


#endif // !_READ_DATASET_

