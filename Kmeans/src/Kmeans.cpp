// Kmeans.cpp: 定义应用程序的入口点。
//

#include "Kmeans.h"

// ================================ xyz file reader, laz file writer
#include <lasreader.hpp>
#include <laswriter.hpp>

// ================================ read all format files in one folder
#include <io.h>
#include <string>
#include <vector>


void GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format)
{
	//文件句柄
	intptr_t hFile = 0;

	//文件信息
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


void readxyz(const std::string& datapath) {

	std::vector<std::string> files;

	std::string format = ".xyz";
	GetAllFormatFiles(datapath, files, format);

	for (std::size_t i = 0; i != files.size(); ++i)
	{
		std::cout << files[i] << '\n';
		/*std::string filename = files[i];
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
		lasreader = nullptr;*/

	}

}


int main()
{
	std::cout << "Hello CMake." << '\n';
	readxyz(DATA_PATH);
	
	
	return 0;

}
