# Kmeans_clustering

About classfication codes: https://desktop.arcgis.com/en/arcmap/latest/manage-data/las-dataset/lidar-point-classification.htm

the input dataset(combining the 500 pointcloud files) -- dataset_truth.laz:

<img width="663" alt="dataset_truth" src="https://user-images.githubusercontent.com/72781910/156752307-a595f68e-2b45-4458-b355-b3e8f5e95239.PNG">

the classification dataset(combining the 500 pointcloud files) -- dataset_result.laz:

<img width="664" alt="dataset_result" src="https://user-images.githubusercontent.com/72781910/156752428-751f3c5a-2f77-4b34-a023-d980eff1fa48.PNG">

**Designed features**:

(1) **area** : (max_x - min_x) * (max_y - min_y)

(2) **height difference** : max_z - min_z

(3) **density** : number of points in one file / area

(4)	**ratio** : x_distance / y_distance (the ratio of width / height)

