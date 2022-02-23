#include "ReadDataset.h"


void ReadDataset::GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format)
{
	//file handle
	intptr_t hFile = 0;

	//file info
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
				}

			}
			else
			{
				files.emplace_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


DatasetPtr ReadDataset::readxyz(const std::string& datapath) {

	std::vector<std::string> files;

	std::string format = ".xyz";
	GetAllFormatFiles(datapath, files, format);

	std::size_t nrows(2076); // to speed up the process of read dataset, obtain the total rows in advance
	std::size_t ncols(3); // 3 dimensions: x, y, z
	DatasetPtr dataset = std::make_shared<Dataset>(nrows, ncols);
	std::cout << "reading dataset..." << '\n';

	std::size_t row_index(0);
	std::size_t x(0), y(1), z(2); // col indexes

	for (std::size_t i = 0; i != files.size(); ++i) // for each pointcloud file, it contains multiple points
	{
		std::cout << files[i] << '\n';
		std::string filename = files[i];
		LASreadOpener lasreadopener;
		lasreadopener.set_file_name(filename.c_str());
		LASreader* lasreader = lasreadopener.open();

		if (!lasreader) {
			std::cerr << "cannot read las file: " << filename << "\n";
			exit(1);
		}

		while (lasreader->read_point()) { 
			// each point is one row in the dataset
			// access the x,y,z data of each point
			// std::cout << lasreader->point.get_x() << " " << lasreader->point.get_y() << " " << lasreader->point.get_z() << '\n';
			// nrows += 1; to obtain the total numbers of rows of the dataset
			// for each row:
			// dataset->data(row_index, 0) - x
			// dataset->data(row_index, 1) - y
			// dataset->data(row_index, 2) - z

			dataset->data(row_index, x) = lasreader->point.get_x();
			dataset->data(row_index, y) = lasreader->point.get_y();
			dataset->data(row_index, z) = lasreader->point.get_z();

			/*
			* Ground truth labels:
			* Classification codes: https://desktop.arcgis.com/en/arcmap/latest/manage-data/las-dataset/lidar-point-classification.htm
			* 000 - 099: building --> cluster: 6 (Building)
			* 100 - 199: car --> cluster: 64 (User Definable)
			* 200 - 299: fence --> cluster: 3 (Low Vegetation)
			* 300 - 399: pole --> cluster: 66 (User Definable)
			* 400 - 499: tree --> cluster: 5 (High Vegetation)
			*/
			/*if (i <= 99)dataset->cluster(row_index) = 6;
			else if (i <= 199)dataset->cluster(row_index) = 54;
			else if (i <= 299)dataset->cluster(row_index) = 3;
			else if (i <= 399)dataset->cluster(row_index) = 66;
			else if (i <= 499)dataset->cluster(row_index) = 5;*/

			++row_index;
		}

		lasreader->close();

		delete lasreader;
		lasreader = nullptr;

	}

	std::cout << "reading dataset done" << '\n';
	std::cout << "number of rows: " << nrows << '\n'; // total rows: 1276507 

	return dataset;

}