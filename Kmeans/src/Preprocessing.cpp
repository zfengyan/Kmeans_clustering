#include "Preprocessing.h"

void Preprocessing::meanNormalize(DatasetPtr dataset){
    ArrayPtr min = std::make_unique<double[]>(dataset->ncols); // typedef std::unique_ptr<double[]> DataPtr;
    ArrayPtr max = std::make_unique<double[]>(dataset->ncols);

    for (std::size_t j = 0; j != dataset->ncols; ++j) { // initialize with the first row
        min[j] = dataset->data(0, j);
        max[j] = dataset->data(0, j);
    }

    for (std::size_t i = 0; i != dataset->nrows; ++i) {
        for (std::size_t j = 0; j != dataset->ncols; ++j) {
            min[j] = std::min(min[j], dataset->data(i, j));
            max[j] = std::max(max[j], dataset->data(i, j));
        }
    }

    // meanNormalization
    // x = (x-min) / (max-min)
    for (std::size_t i = 0; i != dataset->nrows; ++i) {
        for (std::size_t j = 0; j != dataset->ncols; ++j) {
            dataset->data(i, j) = (dataset->data(i, j) - min[j]) / (max[j] - min[j]);
        }
    }
}
