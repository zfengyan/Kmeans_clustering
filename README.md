# Kmeans-clustering

About classfication encoding: https://desktop.arcgis.com/en/arcmap/latest/manage-data/las-dataset/lidar-point-classification.htm

Some data structures referenced : https://github.com/luizgh/knn

## HOW TO USE

`build:` 
Use all files inside [src](https://github.com/SEUZFY/Kmeans_clustering/tree/master/Kmeans/src) folder to build this project. 
Please be aware of the settings of the [CMakeLists.txt](https://github.com/SEUZFY/Kmeans_clustering/blob/master/Kmeans/CMakeLists.txt)
if the program runs across platforms.

`Compile and run:`
After compiling the files, A binary executable file would be obtained. Run it directly and no arguments are required.

## INPUT
It automatically reads in all .xyz files inside [data](https://github.com/SEUZFY/Kmeans_clustering/tree/master/Kmeans/data) folder.

## OUTPUT

The output files are in the same [data](https://github.com/SEUZFY/Kmeans_clustering/tree/master/Kmeans/data) folder as the input files.

[dataset_truth.laz](https://github.com/SEUZFY/Kmeans_clustering/blob/master/Kmeans/data/dataset_truth.laz) -- the ground truth dataset (combining the 500 pointcloud files)

<img width="663" alt="dataset_truth" src="https://user-images.githubusercontent.com/72781910/156752307-a595f68e-2b45-4458-b355-b3e8f5e95239.PNG">

[dataset_result.laz](https://github.com/SEUZFY/Kmeans_clustering/blob/master/Kmeans/data/dataset_result.laz) -- the classification dataset(combining the 500 pointcloud files) 

<img width="664" alt="dataset_result" src="https://user-images.githubusercontent.com/72781910/156752428-751f3c5a-2f77-4b34-a023-d980eff1fa48.PNG">

[report.txt](https://github.com/SEUZFY/Kmeans_clustering/blob/master/Kmeans/data/report.txt) -- the classification statistics report

`Comment`: 

[Manhattan distance](https://iq.opengenus.org/manhattan-distance/) is used by default in this program as the method of calculating distance between feature points.

[Euclidean distance](https://en.wikipedia.org/wiki/Euclidean_distance) has also been tested, the corresponding result files are:

[dataset_result_euclideanDistance.laz](https://github.com/SEUZFY/Kmeans_clustering/blob/master/Kmeans/data/dataset_result_euclideanDistance.laz)

[report_euclidean.txt](https://github.com/SEUZFY/Kmeans_clustering/blob/master/Kmeans/data/report_euclidean.txt)

Generally speaking, after testing, the clustering result using **Manhattan distance** is relatively ideal, so **Manhattan distance** is used as the default method of calculating distance between feature points.

## Designed features:

(1) **area** : (max_x - min_x) * (max_y - min_y)

(2) **height difference** : max_z - min_z

(3) **density** : number of points in one file / area

(4)	**ratio** : x_distance / y_distance (the ratio of width / height)

## Overall statistics:

**Numbers**: 

**Correct numbers**: 349 out of 500

**Wrong numbers**: 151 out of 500

**Accuracy**: 

**Avg accuracy (mAcc)** = 0.776148 -- the average of the accuracy rates for the five categories

**Overall accuracy (OA)** = 0.698 -- total numbers of correct data / total numbers of data
