#include "Kmeans.h"


void Kmenas::clusteringKmeans(DatasetPtr dataset, std::size_t k, std::size_t iterations)
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
