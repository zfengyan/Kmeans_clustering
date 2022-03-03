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
	/*
	* find all certain foramt files inside a certain folder
	* return the number of found files
	*/
	static std::size_t GetAllFormatFiles(
		const std::string& path, 
		std::vector<std::string>& files, 
		const std::string& format);

	/*
	* get the numbers of all points in all xyz files
	*/
	static std::size_t GetAllpoints(const std::string& path);

public:

	/*
	* read all xyz files and return a pointer
	*/
	static std::pair<DatasetPtr, DatasetPtr> readxyz(
		const std::string& datapath,
		std::size_t& numFeaturePoints);

};


#endif // !_READ_DATASET_

