# CMakeList.txt: Kmeans 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# ecternal dependencies
add_subdirectory(extern/LAStools) 

add_definitions(
  -DDATA_PATH=\"${PROJECT_SOURCE_DIR}/Kmeans/testdata\"
)

# 将源代码添加到此项目的可执行文件。
add_executable (Kmeans "src/main.cpp"  "src/ReadDataset.h" "src/ReadDataset.cpp" "src/Dataset.h" "src/Dataset.cpp" "src/Preprocessing.h" "src/Preprocessing.cpp" "src/WriteDataset.h" "src/WriteDataset.cpp"  "src/Kmeans.h"  "src/Kmeans.cpp")

# TODO: 如有需要，请添加测试并安装目标。
target_link_libraries(Kmeans PRIVATE
  LASlib
)

set_target_properties(Kmeans PROPERTIES 
  CXX_STANDARD 11
)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Kmeans)
