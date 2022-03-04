#include "WriteDataset.h"

/*
* write original ground truth point cloud to the file
*/
void WriteDataset::output_origin_dataset(const std::string& filepath, DatasetPtr dataset){
    /*
    Function to write a new LAS/LAZ file with point labels (for the classification field)
    Inputs:
    filename:   the filename to write the LAS file to
    dataset :   input dataset
    Labels  :   Uses LAS/LAZ classification codes: https://desktop.arcgis.com/en/arcmap/latest/manage-data/las-dataset/lidar-point-classification.htm
    */
    std::string filename = filepath + "/dataset_true.laz";

    LASwriteOpener laswriteopener;
    laswriteopener.set_file_name(filename.c_str());

    LASheader lasheader;
    lasheader.x_scale_factor = 0.01; // 0.01
    lasheader.y_scale_factor = 0.01;
    lasheader.z_scale_factor = 0.01;
    lasheader.x_offset = 0.0;
    lasheader.y_offset = 0.0;
    lasheader.z_offset = 0.0;
    lasheader.point_data_format = 0;
    lasheader.point_data_record_length = 20;

    LASpoint laspoint;
    laspoint.init(&lasheader, lasheader.point_data_format, lasheader.point_data_record_length, 0);

    LASwriter* laswriter = laswriteopener.open(&lasheader);
    if (laswriter == 0)
    {
        std::cerr << "ERROR: could not open laswriter\n";
        exit(1);
    }

    std::size_t x(0), y(1), z(2); // col index(attribute)
    for (std::size_t i = 0; i != dataset->nrows; ++i) { // each row: each point
        
        // set xyz of points(the xyz coordinates are preprocessed)
        laspoint.set_x(dataset->data(i, x));
        laspoint.set_y(dataset->data(i, y));
        laspoint.set_z(dataset->data(i, z));

        // set classification
        laspoint.set_classification((int)dataset->truthid(i));

        // write points
        laswriter->write_point(&laspoint);
        laswriter->update_inventory(&laspoint);
    }

    laswriter->update_header(&lasheader, TRUE);
    laswriter->close();
    delete laswriter;
    laswriter = nullptr;
}


/*
* set the cluster results of origin_dataset
* according to the result of clustering_dataset
* linked by: fileid -- indicating the point belongs to which file
* 
* NB: this function needs to be called after Evaluation::evaluation(clustering_dataset, k, result_pair) fucntion
* because in the evaluation() fucntion, the clusters of clustering_dataset get assigned
* by choosing the "majority"
*/
void WriteDataset::assign_result_dataset(DatasetPtr origin_dataset, DatasetPtr clustering_dataset)
{
    for (std::size_t i = 0; i != origin_dataset->nrows; ++i)
    {
        std::size_t Fileid = origin_dataset->fileid(i); // get the file id
        origin_dataset->cluster(i) = clustering_dataset->cluster(Fileid); // assign the clsutering result
    }
}


/*
 * write classification result of the original point cloud to the file
 * NB: this function needs to be called after the assign_result_dataset() function
 */
void WriteDataset::output_result_dataset(const std::string& filepath, DatasetPtr dataset)
{
    /*
    Function to write a new LAS/LAZ file with point labels (for the classification field)
    Inputs:
    filename:   the filename to write the LAS file to
    dataset :   input dataset
    Labels  :   Uses LAS/LAZ classification codes: https://desktop.arcgis.com/en/arcmap/latest/manage-data/las-dataset/lidar-point-classification.htm
    */
    std::string filename = filepath + "/dataset_result.laz";

    LASwriteOpener laswriteopener;
    laswriteopener.set_file_name(filename.c_str());

    LASheader lasheader;
    lasheader.x_scale_factor = 0.01; // 0.01
    lasheader.y_scale_factor = 0.01;
    lasheader.z_scale_factor = 0.01;
    lasheader.x_offset = 0.0;
    lasheader.y_offset = 0.0;
    lasheader.z_offset = 0.0;
    lasheader.point_data_format = 0;
    lasheader.point_data_record_length = 20;

    LASpoint laspoint;
    laspoint.init(&lasheader, lasheader.point_data_format, lasheader.point_data_record_length, 0);

    LASwriter* laswriter = laswriteopener.open(&lasheader);
    if (laswriter == 0)
    {
        std::cerr << "ERROR: could not open laswriter\n";
        exit(1);
    }

    std::size_t x(0), y(1), z(2); // col index(attribute)
    for (std::size_t i = 0; i != dataset->nrows; ++i) { // each row: each point

        // set xyz of points(the xyz coordinates are preprocessed)
        laspoint.set_x(dataset->data(i, x));
        laspoint.set_y(dataset->data(i, y));
        laspoint.set_z(dataset->data(i, z));

        // set classification
        laspoint.set_classification((int)dataset->cluster(i));

        // write points
        laswriter->write_point(&laspoint);
        laswriter->update_inventory(&laspoint);
    }

    laswriter->update_header(&lasheader, TRUE);
    laswriter->close();
    delete laswriter;
    laswriter = nullptr;
}
