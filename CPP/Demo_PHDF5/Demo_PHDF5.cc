#include <cassert>
#include <iostream>
#include <array>
#include <string>
#include <mpi.h>
#include <hdf5.h>
#include "H5_mpio.hh"
#include "Macros.hh"

using namespace std;

#define FILENAME "TestFile.h5"
#define DSETNAME "TestDset"
#define SIZE     10



/* =============================================================================
 * Routine to check whether an HDF5 property list generates a parallel operation
 * and whether any I/O optimizations are performed
 * =============================================================================*/
void io_check(const hid_t &pl_id, const string &what) {
    H5D_mpio_actual_io_mode_t actual_io_mode;
    CHECK_ERROR(H5Pget_mpio_actual_io_mode(pl_id, &actual_io_mode));

    H5D_mpio_actual_chunk_opt_mode_t actual_chunk_opt_mode;
    CHECK_ERROR(H5Pget_mpio_actual_chunk_opt_mode(pl_id, &actual_chunk_opt_mode));

    uint32_t no_collective_cause_local, no_collective_cause_global;
    CHECK_ERROR(H5Pget_mpio_no_collective_cause(pl_id,
                 &no_collective_cause_local, &no_collective_cause_global));

    if (actual_io_mode             == H5D_MPIO_NO_COLLECTIVE         or
        actual_chunk_opt_mode      == H5D_MPIO_NO_CHUNK_OPTIMIZATION or
        no_collective_cause_local  != H5D_MPIO_COLLECTIVE            or
        no_collective_cause_global != H5D_MPIO_COLLECTIVE)
    {
        WARNING("[" << what << "] Actual I/O mode, chunk optimization and local and global non-collective I/O causes: " <<
                 H5D_mpio_actual_io_mode_map.at(actual_io_mode)                    << ", " <<
                 H5D_mpio_actual_chunk_opt_mode_map.at(actual_chunk_opt_mode)      << ", " <<
                 H5Pget_mpio_no_collective_cause_map.at(no_collective_cause_local) << ", " <<
                 H5Pget_mpio_no_collective_cause_map.at(no_collective_cause_global));
    }

    else {
        INFO("[" << what << "] Actual I/O mode, chunk optimization and local and global non-collective I/O causes: " <<
              H5D_mpio_actual_io_mode_map.at(actual_io_mode)                    << ", " <<
              H5D_mpio_actual_chunk_opt_mode_map.at(actual_chunk_opt_mode)      << ", " <<
              H5Pget_mpio_no_collective_cause_map.at(no_collective_cause_local) << ", " <<
              H5Pget_mpio_no_collective_cause_map.at(no_collective_cause_global));
    }
}





/* =============================================================================
 * Main routine
 * =============================================================================*/
int main(int argc, char **argv) {
    // Initialize MPI
    CHECK_ERROR(MPI_Init(&argc, &argv));

    // Get the total number of ranks and the ID of the current rank
    int nranks, rankid;
    CHECK_ERROR(MPI_Comm_size(MPI_COMM_WORLD, &nranks));
    CHECK_ERROR(MPI_Comm_rank(MPI_COMM_WORLD, &rankid));

    /* Create a file access property list with parallel I/O access using the
     * world communicator and the null MPI_Info object                          */
    auto fapl_id = H5Pcreate(H5P_FILE_ACCESS);
    assert(fapl_id >= 0);
    CHECK_ERROR(H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL));

    // Collectively create a new file (overwrite if present)
    const auto file_create_id = H5Fcreate(FILENAME, H5F_ACC_TRUNC,
                                          H5P_DEFAULT, fapl_id);
    assert(file_create_id >= 0);


    /* Create a file dataspace describing a 2D dataset with nranks rows and SIZE
     * columns                                                                  */
    const hsize_t fdims[2]  = {nranks, SIZE};
    const auto    fspace_id = H5Screate_simple(2, fdims, nullptr);
    assert(fspace_id >= 0);

    /* Each MPI rank will write a 1D array of size SIZE in the rankid-th column
     * of the dataset. Therefore, the memory dataspace -- local to each MPI
     * rank -- is 1D and has SIZE elements.                                     */
    const hsize_t mdims     = SIZE;
    const auto    mspace_id = H5Screate_simple(1, &mdims, nullptr);
    assert(mspace_id >= 0);


    /* Select the hyperslab in the file which should be written by the current
     * MPI rank
     * NOTE: first index is row, second index is column
     * NOTE: fblock and fstride could also be left as nullptr, as that would
     *       default to 1 in each direction (i.e. contiguous dataset with block
     *       size equal to 1)                                                   */
    const hsize_t fstart[2]  = {rankid, 0};
    const hsize_t fstride[2] = {1, 1};
    const hsize_t fcount[2]  = {1, SIZE};
    const hsize_t fblock[2]  = {1, 1};

    CHECK_ERROR(H5Sselect_hyperslab(fspace_id, H5S_SELECT_SET,
                                    fstart, fstride, fcount, fblock));


    /* Select the hyperslab in memory which should be written by the current MPI
     * rank -- it's just the full memory dataspace                              */
    const hsize_t mstart  = 0;
    const hsize_t mstride = 1;
    const hsize_t mcount  = SIZE;
    const hsize_t mblock  = 1;

    CHECK_ERROR(H5Sselect_hyperslab(mspace_id, H5S_SELECT_SET,
                                   &mstart, &mstride, &mcount, &mblock));


    /* Create the dataset using the default link creation, dataset creation and
     * dataset access property list identifiers
     * NOTE: all MPI ranks must create the dataset                              */
    const auto dset_create_id = H5Dcreate(file_create_id, DSETNAME,
                                          H5T_NATIVE_INT, fspace_id,
                                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    assert(dset_create_id >= 0);


    // Create a collective dataset transfer property list
    const auto dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    assert(dxpl_id >= 0);
    CHECK_ERROR(H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE));


    // Generate and write some data to the dataset
    array<int, SIZE> data;

    for (int i = 0; i < SIZE; ++i) {
        data.at(i) = rankid + i;
    }

    CHECK_ERROR(H5Dwrite(dset_create_id, H5T_NATIVE_INT, mspace_id, fspace_id,
                         dxpl_id, data.data()));

    CHECK_ERROR(H5Dclose(dset_create_id));
    CHECK_ERROR(H5Sclose(mspace_id));
    CHECK_ERROR(H5Sclose(fspace_id));
    CHECK_ERROR(H5Fclose(file_create_id));


    // Check that the writing was really parallel
    io_check(dxpl_id, "Writing dataset");

    // Print some info
    INFO("Dataset successfully written to disk");



    // -------------------------------------------------------------------------



    // Re-open the file using the same file access property list used to create it
    const auto file_id_open = H5Fopen(FILENAME, H5F_ACC_RDONLY, fapl_id);
    assert(file_id_open >= 0);

    // Open the dataset *INDEPENDENTLY* of all other MPI ranks (-> H5P_DEFAULT)
    const auto dset_read_id = H5Dopen(file_id_open, DSETNAME, H5P_DEFAULT);
    assert(dset_read_id >= 0);


    // Retrieve the dimensionality of the dataset and the number of elements in it
    const auto dspace_id = H5Dget_space(dset_read_id);
    assert(dspace_id >= 0);

    const auto ndims = H5Sget_simple_extent_ndims(dspace_id);
    assert(ndims == 2);

    hsize_t dims[2];
    const auto ndims_again = H5Sget_simple_extent_dims(dspace_id, dims, nullptr);
    assert(ndims_again == 2);

    INFO("Dataset dimensions from reading: " << dims[0] << "*" << dims[1]);
    CHECK_ERROR(H5Sclose(dspace_id));


    // Read the dataset into a buffer in parallel
    int *buffer = new int[dims[0]*dims[1]];
    CHECK_ERROR(H5Dread(dset_read_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, dxpl_id, buffer));
    CHECK_ERROR(H5Dclose(dset_read_id));
    CHECK_ERROR(H5Pclose(fapl_id));
    CHECK_ERROR(H5Fclose(file_id_open));

    // Check that the reading was really parallel
    io_check(dxpl_id, "Reading dataset");


    // Print the dataset to screen
    if (rankid == 0) {
        cout << endl;
        for (int i = 0; i < nranks; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                cout << buffer[i*SIZE + j] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    delete [] buffer;
    buffer = nullptr;
    CHECK_ERROR(H5Pclose(dxpl_id));


    // Finalize MPI
    CHECK_ERROR(MPI_Finalize());
    
    return 0;
}
