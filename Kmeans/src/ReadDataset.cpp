#include "ReadDataset.h"

/*
* get all certain format files in one data folder
*/
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


/*
* read all 500 files into a dataset
*/
std::pair<DatasetPtr, DatasetPtr> ReadDataset::readxyz(
	const std::string& datapath,
	std::size_t p_nrows,
	std::size_t p_ncols,
	std::size_t p_num_rows,
	std::size_t p_num_attributes) 
{
	std::vector<std::string> files;

	std::string format = ".xyz";
	GetAllFormatFiles(datapath, files, format);

	std::size_t nrows(p_nrows); // to speed up the process of read dataset, obtain the total rows in advance
	std::size_t ncols(p_ncols); // 3 dimensions: x, y, z
	DatasetPtr origin_dataset = std::make_shared<Dataset>(nrows, ncols);
	
	std::cout << "reading dataset..." << '\n';

	std::size_t row_index(0); // track the record id in the origin dataset
	std::size_t x(0), y(1), z(2); // col indexes

	// clustering dataset: 500*n
	DatasetPtr clustering_dataset = std::make_shared<Dataset>(p_num_rows, p_num_attributes);

	for (std::size_t i = 0; i != files.size(); ++i) // for each pointcloud file, it contains multiple points, there are 500 files
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


		// used to store all the points in one file
		// to calculate attributes
		std::vector<Point> points_in_one_file;

		// read in points in one pcloud file
		double count_points(0); // count the number of points in one single file
		while (lasreader->read_point()) { 
			// each point is one row in the dataset
			// access the x,y,z data of each point
			// std::cout << lasreader->point.get_x() << " " << lasreader->point.get_y() << " " << lasreader->point.get_z() << '\n';
			// nrows += 1; to obtain the total numbers of rows of the dataset
			// for each row:
			// dataset->data(row_index, 0) - x
			// dataset->data(row_index, 1) - y
			// dataset->data(row_index, 2) - z

			origin_dataset->data(row_index, x) = lasreader->point.get_x();
			origin_dataset->data(row_index, y) = lasreader->point.get_y();
			origin_dataset->data(row_index, z) = lasreader->point.get_z();

			// add points to the points vector
			Point p(lasreader->point.get_x(), lasreader->point.get_y(), lasreader->point.get_z());
			points_in_one_file.emplace_back(p);

			/*
			* Ground truth labels:
			* Classification codes: https://desktop.arcgis.com/en/arcmap/latest/manage-data/las-dataset/lidar-point-classification.htm
			* 000 - 099: building --> cluster: 6 (Building)
			* 100 - 199: car --> cluster: 64 (User Definable)
			* 200 - 299: fence --> cluster: 3 (Low Vegetation)
			* 300 - 399: pole --> cluster: 66 (User Definable)
			* 400 - 499: tree --> cluster: 5 (High Vegetation)
			*/
			if (i <= 99)origin_dataset->truthid(row_index) = 6;
			else if (i <= 199)origin_dataset->truthid(row_index) = 54;
			else if (i <= 299)origin_dataset->truthid(row_index) = 3;
			else if (i <= 399)origin_dataset->truthid(row_index) = 66;
			else if (i <= 499)origin_dataset->truthid(row_index) = 5;

			origin_dataset->fileid(row_index) = i; // i is the file index(0~500)

			++row_index;
			++count_points;
		} // end while : all the points in one file

		// close the file
		lasreader->close();

		delete lasreader;
		lasreader = nullptr;


		/*
		process points in each file
		design attributes here
		*/
		double max_x(0), min_x(_INFINITE_); // maximum and minimum x, NB: different initialized values
		double max_y(0), min_y(_INFINITE_); // maximum and minimum y
		double max_z(0), min_z(_INFINITE_); // maximum and minimum z

		//double accumulate_x(0), accumulate_y(0);
		
		// get the maximum and minimum xyz values
		for (auto& p : points_in_one_file)
		{
			max_x = p.x > max_x ? p.x : max_x;
			min_x = p.x < min_x ? p.x : min_x;

			max_y = p.y > max_y ? p.y : max_y;
			min_y = p.y < min_y ? p.y : min_y;

			max_z = p.z > max_z ? p.z : max_z;
			min_z = p.z < min_z ? p.z : min_z;

			//accumulate_x += p.x;
			//accumulate_y += p.y;
		}


		/*
		* designed attributes
		*/
		double area = (max_x - min_x) * (max_y - min_y); // attribute 1
		double height_diff = max_z - min_z; // attribute 2
		double density = area > 0 ? (count_points / area) : 0; // attribute 3
		double ratio = (max_y - min_y) > 0 ? ((max_x - min_x) / (max_y - min_y)) : 0; // attribute 4


		/*
		* assign designed attributes
		* 
		* clustering_dataset->data(i, j)-- > 500 fies, 500 feature points
		* for each feature point : data(i, j) -- j is the column(attribute)
		* one file corresponds to one feature point
		*/
		clustering_dataset->data(i, 0) = area;
		clustering_dataset->data(i, 1) = height_diff;
		clustering_dataset->data(i, 2) = density;
		clustering_dataset->data(i, 3) = ratio;

		clustering_dataset->fileid(i) = i;

		if (i <= 99)clustering_dataset->truthid(i) = 6;
		else if (i <= 199)clustering_dataset->truthid(i) = 54;
		else if (i <= 299)clustering_dataset->truthid(i) = 3;
		else if (i <= 399)clustering_dataset->truthid(i) = 66;
		else if (i <= 499)clustering_dataset->truthid(i) = 5;


	} // end for : all 500 files

	std::cout << "reading dataset done" << '\n';
	std::cout << "number of rows(points) of original dataset(all the points in 500 point cloud files): " 
		<< nrows << '\n'; // total rows: 1276507 
	std::cout << '\n';

	return std::make_pair(origin_dataset, clustering_dataset);

}

