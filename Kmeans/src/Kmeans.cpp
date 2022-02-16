// Kmeans.cpp: 定义应用程序的入口点。
//

#include "Kmeans.h"

// ================================ xyz file reader, laz file writer
#include <lasreader.hpp>
#include <laswriter.hpp>

// ================================
#include <filesystem>
namespace fs = std::filesystem;


void readxyz(const std::string& datapath) {
	std::string fname = "/001.xyz";
	std::string filename = datapath + fname;
	
	LASreadOpener lasreadopener;
	lasreadopener.set_file_name(filename.c_str());
	LASreader* lasreader = lasreadopener.open();

	if (!lasreader) {
		std::cerr << "cannot read las file: " << filename << "\n";
		exit(1);
	}


	while (lasreader->read_point()) {
		std::cout << lasreader->point.get_x() << " " << lasreader->point.get_y() << " " << lasreader->point.get_z() << '\n';
	}

	lasreader->close();

	delete lasreader;
	lasreader = nullptr;


}

int main()
{
	std::cout << "Hello CMake." << '\n';
	readxyz(DATA_PATH);
	//std::cout << DATA_PATH;
	return 0;
}
