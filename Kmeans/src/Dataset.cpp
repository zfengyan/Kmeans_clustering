#include "Dataset.h"

Dataset::Dataset(std::size_t rows, std::size_t cols): Matrix(rows, cols){
    Cluster = std::make_unique<std::size_t[]>(rows);
    for (std::size_t i = 0; i != rows; ++i)Cluster[i] = 0; // initialize with 0

    Min_dist = std::make_unique<double[]>(rows);
    for (std::size_t i = 0; i != rows; ++i)Min_dist[i] = _INFINITE_; // initialize with INFINITE

    Fileid = std::make_unique<std::size_t[]>(rows);
    for (std::size_t i = 0; i != rows; ++i)Fileid[i] = 0; // initialize with 0

    Truthid = std::make_unique<std::size_t[]>(rows);
    for (std::size_t i = 0; i != rows; ++i)Truthid[i] = 0; // initialize with 0
}

double Dataset::euclideanDistance(std::size_t row_a, std::size_t row_b){
    double squared_dist(0);
    double sum(0);
    for (std::size_t j = 0; j < ncols; ++j) {
        squared_dist = (data(row_a, j) - data(row_b, j)) * (data(row_a, j) - data(row_b, j));
        sum += squared_dist;
    }
    return sqrt(sum);
}

double Dataset::euclideanDistance(DatasetPtr centroids, std::size_t centerid, std::size_t row)
{
    double squared_dist(0);
    double sum(0);
    for (std::size_t j = 0; j < ncols; ++j) {
        squared_dist = (centroids->data(centerid, j) - data(row, j)) * (centroids->data(centerid, j) - data(row, j));
        sum += squared_dist;
    }
    return sqrt(sum);
}

double Dataset::manhattanDistance(std::size_t row_a, std::size_t row_b)
{
    double sum(0);
    for (std::size_t j = 0; j < ncols; ++j) {
        sum += abs(data(row_a, j) - data(row_b, j));
    }
    return sum;
}

double Dataset::manhattanDistance(DatasetPtr centroids, std::size_t centerid, std::size_t row)
{
    double sum(0);
    for (std::size_t j = 0; j < ncols; ++j) {
        sum += abs(centroids->data(centerid, j) - data(row, j));
    }
    return sum;
}

