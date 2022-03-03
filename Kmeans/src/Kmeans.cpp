#include "Kmeans.h"


void Kmeans::clusteringKmeans(DatasetPtr dataset, std::size_t k, std::size_t iterations)
{
    std::size_t n(dataset->nrows);
    assert(k <= n);

    /*
    randomly select the initial clustering points
    using std::random_shuffle
    because the index of initial clustering rows(points) is not allowed to repeate
    */
    std::vector<std::size_t> centroid_index_in_origin;
    centroid_index_in_origin.reserve(n);
    for (std::size_t i = 0; i != n; ++i)centroid_index_in_origin.emplace_back(i);
    std::random_shuffle(centroid_index_in_origin.begin(), centroid_index_in_origin.end()); // used in C++ 11 not in 14 and 17


    // centroid_index[i]: selected centroid row_index
    //for(std::size_t i = 0; i != k; ++i)
    //{
    //     std::cout<<centroid_index_in_origin[i]<<'\n'; // --> the index of selected k initial rows(points)
    //}

    //used for store the k centroid points(rows)
    //centroids: k*ncols matrix with some attributes
    DatasetPtr centroids = std::make_shared<Dataset>(k, dataset->ncols);

    //intialize k points with selected row indexes in the original dataset
    for (std::size_t centerid = 0; centerid != k; ++centerid)
    {
        for (std::size_t j = 0; j != dataset->ncols; ++j) // each column(in each row)
        {
            std::size_t center_in_origin = centroid_index_in_origin[centerid]; // row id
            centroids->data(centerid, j) = dataset->data(center_in_origin, j);
        }

    }


    // iterate the kmeans clustering process
    for (std::size_t i = 0; i != iterations; ++i)
    {
        // Assigning records(rows) to a cluster
    // iterate by the defined dist
        for (std::size_t centerid = 0; centerid != k; ++centerid) {
            for (std::size_t row = 0; row != dataset->nrows; ++row) { // for each point(row)

                double dist(0);
                dist = dataset->manhattanDistance(centroids, centerid, row); // calculate the distance between centerid and this row(point)
                
                if (dist < dataset->min_dist(row)) {
                    dataset->min_dist(row) = dist;
                    dataset->cluster(row) = centerid;
                    
                }
            }
        }


        // Computing new centroids: mean coordinates of all the records in a cluster


        // accumulation of "point" numbers of one cluster
        // ie cluster_numbers[k] stands for: there are total "cluster_numbers[k]" points in cluster k
        std::vector<std::size_t> cluster_numbers;
        cluster_numbers.reserve(k); // there shoule be k elements in cluster_numbers

        /*
        ie there are two attributes xand y in the dataset, and std::vector<double> sumX, std::vector<double> sumY
        sumX should contains k elements, assume k=3 --> 3 clusters in total
        sumX[0]: accumulated x for cluster 0, sumY[0]: accumulated y for cluster 0
        sumX[1]: accumulated x for cluster 1, sumY[1]: accumulated y for cluster 1
        sumX[2]: accumulated x for cluster 2, sumY[2]: accumulated y for cluster 2
        attributes: each element is a std::vector<double>, indicating an attribute

        ie, attributes[0] : sumX, attributes[1] : sumY
        --> attributes[0][0] : accumulated x for cluster 0
        --> attributes[0][1] : accumulated x for cluster 1
        --> attributes[1][0] : accumulated y for cluster 0
        */
        std::vector<std::vector<double>> attributes; // each element stands for one attribute
        attributes.reserve(dataset->ncols); // there should be dataset->ncols attributes in total

        // initialize: add each attribute to the attributes vector
        for (std::size_t j = 0; j != dataset->ncols; ++j) {
            std::vector<double> one_attribute;
            one_attribute.reserve(k); // in each attribute, there should be k elements in total

            // initialize each one_attribute vector with 0
            for (std::size_t num = 0; num != k; ++num)
            {
                one_attribute.emplace_back(0);
            }

            attributes.emplace_back(one_attribute);
        }

        // initialize: set the initial value of cluster_numbers as 0
        for (std::size_t i = 0; i != k; ++i) {
            cluster_numbers.emplace_back(0); // cluster_numbers[0]: stands for the numbers of cluster 0

        }


        // traverse each record and reset the min_dist to _INFINITE_
        for (std::size_t row = 0; row != dataset->nrows; ++row) {

            std::size_t cluster_id(dataset->cluster(row)); // cluster_id of current row
            cluster_numbers[cluster_id] += 1; // accumulate the numbers of cluster: cluster_id

            // for each row(point), traverse its attributes
            for (std::size_t col = 0; col != dataset->ncols; ++col) {
                attributes[col][cluster_id] += dataset->data(row, col); // accumulate the values of each attribute
            }

            dataset->min_dist(row) = _INFINITE_; // reset the min_dist value for each row after accumulation obtained

        }


        // calculate the new centroids
        // clusterid is centerid: 0, 1, ..., k-1
        for (std::size_t centerid = 0; centerid != centroids->nrows; ++centerid)
        {
            for (std::size_t col = 0; col != centroids->ncols; ++col)
            {
                centroids->data(centerid, col) = attributes[col][centerid] / cluster_numbers[centerid];

            }

        } // end for: for each centroid


    } // end for: iterations


} // end for: fucntion clustering


void Evaluation::evaluation(DatasetPtr clustering_dataset, std::size_t k, std::vector<double>& accuracy)
{
    /*
    * select the majority of the clustering results as labels
    */

    // count[k] : store all the ids belongs to cluster k
    std::vector<std::vector<std::size_t>> count;

    // initialize
    for (std::size_t i = 0; i != k; ++i)
    {
        std::vector<std::size_t> each_cluster;
        count.emplace_back(each_cluster);
    }

    // for each record(feature point)
    for (std::size_t i = 0; i != clustering_dataset->nrows; ++i) 
    {
        for (std::size_t cid = 0; cid != k; ++cid)
        {
            if (clustering_dataset->cluster(i) == cid)count[cid].emplace_back(i);
        }

    } // end for: each record(feature points)

    int a[5]{}; // the ground truth
    a[0] = 6;
    a[1] = 54;
    a[2] = 3;
    a[3] = 66;
    a[4] = 5;
    std::cout << "------------------------" << " "
        << "mapping for the biggest type and ground truth labels: "
        << " " << "------------------------" << '\n';

    std::cout << "type: " << 0 << " " << " " << "truth: " << a[0] << "  " << "building" << '\n';

    std::cout << "type: " << 1 << " " << " " << "truth: " << a[1] << "  " << "car" << '\n';

    std::cout << "type: " << 2 << " " << " " << "truth: " << a[2] << "  " << "fence" << '\n';

    std::cout << "type: " << 3 << " " << " " << "truth: " << a[3] << "  " << "pole" << '\n';

    std::cout << "type: " << 4 << " " << " " << "truth: " << a[4] << "  " << "tree" << '\n';

    std::cout << '\n';
    
    int current_cluster_group(0); // for output
    for (auto& each_cluster : count) // for each cluster
    {
        int b[5]{}; // count for each label

        for (auto& index : each_cluster) // for each row in one cluster
        {
            if (clustering_dataset->truthid(index) == a[0])b[0] += 1;
            else if (clustering_dataset->truthid(index) == a[1])b[1] += 1;
            else if (clustering_dataset->truthid(index) == a[2])b[2] += 1;
            else if (clustering_dataset->truthid(index) == a[3])b[3] += 1;
            else if (clustering_dataset->truthid(index) == a[4])b[4] += 1;
        } // end for: each row in one clusters

        std::cout << "------------------------" << " " << "Cluster group: "
            << " " << current_cluster_group << " " << "------------------------" << '\n';
        std::cout << "numbers of each truth label: " << '\n';

        std::cout << "type: " << 0 << " " << " " << "truth: " << a[0]
            << " " << " " << "numbers: " << b[0] << '\n';
        std::cout << "type: " << 1 << " " << " " << "truth: " << a[1]
            << " " << " " << "numbers: " << b[1] << '\n';
        std::cout << "type: " << 2 << " " << " " << "truth: " << a[2]
            << " " << " " << "numbers: " << b[2] << '\n';
        std::cout << "type: " << 3 << " " << " " << "truth: " << a[3]
            << " " << " " << "numbers: " << b[3] << '\n';
        std::cout << "type: " << 4 << " " << " " << "truth: " << a[4]
            << " " << " " << "numbers: " << b[4] << '\n';

        // sort
        // NOT sort the elements in array b, sort b[0]~b[5]
        int biggest(0);
        int biggest_type(0);

        biggest = biggest > b[0] ? biggest : b[0];
        biggest = biggest > b[1] ? biggest : b[1];
        biggest = biggest > b[2] ? biggest : b[2];
        biggest = biggest > b[3] ? biggest : b[3];
        biggest = biggest > b[4] ? biggest : b[4];

        if (biggest == b[0])biggest_type = 0;
        else if (biggest == b[1])biggest_type = 1;
        else if (biggest == b[2])biggest_type = 2;
        else if (biggest == b[3])biggest_type = 3;
        else if (biggest == b[4])biggest_type = 4;

        std::cout << "type with the biggest numbers: " << biggest_type << '\n';
        std::cout << "selected cluster result for current group: " << a[biggest_type] << '\n';
        std::cout << '\n';

        // set the majority as the classification result
        for (auto& index : each_cluster) // for each row in one cluster
        {
            clustering_dataset->cluster(index) = a[biggest_type];
        }

        /*
        * calculate accuracy for each cluster group
        */
        double correct(0); // count: cluster == truth
        double wrong(0); // count: cluster != truth
        for (auto& index : each_cluster) // for each row in one cluster
        {
            if (clustering_dataset->cluster(index) == clustering_dataset->truthid(index))++correct;
            else ++wrong;
        }
        std::cout << "Accuracy: " << '\n';
        std::cout << "Correct results: " << correct << '\n';
        std::cout << "Wrong results: " << wrong << '\n';

        double accuracy_each_group = correct / (correct + wrong);
        accuracy.emplace_back(accuracy_each_group); // add it to the accuracy vector

        std::cout << "accuracy = " << accuracy_each_group << '\n';
        std::cout << '\n';

        ++current_cluster_group;
    } // end for: each cluster

}
