#ifndef _DATASET_
#define _DATASET_

// ================================ pointer
#include <memory>

// ================================ constant
constexpr auto _INFINITE_ = 9999;

// ================================ typedef
class Dataset;
typedef std::unique_ptr<double[]> ArrayPtr; // DataPtr: a pointer pointing to an 1-D double array
typedef std::shared_ptr<Dataset> DatasetPtr;
typedef std::unique_ptr<std::size_t[]> ClusteridPtr;


class Matrix {
private:
    ArrayPtr Data; // Data: a unique_ptr pointing to an 1-D double type array
public:
    std::size_t nrows;
    std::size_t ncols;
public:
    Matrix(std::size_t rows, std::size_t cols) : // cosnt qualifier: parameters are read-only
        nrows(rows),
        ncols(cols) {
        Data = std::make_unique<double[]>(nrows * ncols);
    }

    double& data(std::size_t row, std::size_t col) {
        return Data[row * ncols + col];
    }

    virtual ~Matrix() = default;

};


class Dataset : public Matrix {
private:
    ClusteridPtr Cluster; // cluster result of each row
    ArrayPtr Min_dist; // minimum distance of each row
public:
    Dataset(std::size_t rows, std::size_t cols);

    std::size_t& cluster(std::size_t row) { return Cluster[row]; } // declaration and definition inside .h file(inline suggested)

    double& min_dist(std::size_t row) { return Min_dist[row]; }

    void shuffle() {} // randomly arrange the original data

    double euclideanDistance(std::size_t row_a, std::size_t row_b);

    double manhattanDistance(std::size_t row_a, std::size_t row_b);

    virtual ~Dataset() = default;
};


#endif // !_DATASET_

