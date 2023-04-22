#include <cassert>
#include <iostream>
#include <array>
#include <mpi.h>
#include <hdf5.h>

using namespace std;

#define FILENAME "TestFile.h5"
#define DSETNAME "TestDset"
#define SIZE     3


#define H5_CHECK(H5call)                         \
do {                                             \
    const int err = H5call;                      \
    if (err < 0) {                               \
        cout << "HDF5 call '" << #H5call         \
             << "' returned error code " << err  \
             << " (line " << __LINE__ << ")"     \
             << endl;                            \
    }                                            \
} while (0)



int main(int argc, char **argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the total number of ranks and the ID of the current rank
    int nranks, rankid;
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankid);

    /* Create a file access property list with parallel I/O access using the
     * world communicator and the null MPI_Info object                          */
    auto fapl_id = H5Pcreate(H5P_FILE_ACCESS);
    assert(fapl_id >= 0);
    H5_CHECK(H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL));

    // Collectively create a new file (overwrite if present)
    const auto file_id_create = H5Fcreate(FILENAME, H5F_ACC_TRUNC,
                                          H5P_DEFAULT, fapl_id);
    assert(file_id_create >= 0);


    // Create the memory and file dataspaces for the datasets
    hsize_t dim = SIZE;

    const auto mspace_id = H5Screate_simple(nranks, &dim, nullptr);
    assert(mspace_id >= 0);

    const auto fspace_id = H5Screate_simple(nranks, &dim, nullptr);
    assert(fspace_id >= 0);


    /* Create a one-dimensional dataset of size SIZE using the default link
     * creation, dataset creation and dataset access property list identifiers  */
    const auto dset_id = H5Dcreate(file_id_create, DSETNAME, H5T_NATIVE_INT, fspace_id,
                                   H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    assert(dset_id >= 0);

    // Create a collective dataset transfer property list
    const auto dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    assert(dxpl_id >= 0);
    H5_CHECK(H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE));

    // Generate the data to be written to the dataset
    array<int, SIZE> data; 
    for (int i = rankid; i < rankid + SIZE; ++i) {
        data.at(i) = i;
    }

    // Write the data to the dataset
    H5_CHECK(H5Dwrite(dset_id, H5T_NATIVE_INT, mspace_id, fspace_id, dxpl_id, data.data()));

    // Close everything
    H5_CHECK(H5Dclose(dset_id));
    H5_CHECK(H5Sclose(mspace_id));
    H5_CHECK(H5Sclose(fspace_id));
    H5_CHECK(H5Pclose(dxpl_id));
    H5_CHECK(H5Fclose(file_id_create));


    // Re-open the file using the same file access property list
    const auto file_id_open = H5Fopen(FILENAME, H5F_ACC_RDONLY, fapl_id);
    assert(file_id_open != H5I_INVALID_HID);

    // Close the file
    H5_CHECK(H5Fclose(file_id_open));

    // Close the file access property list
    H5_CHECK(H5Pclose(fapl_id));

    // Finalize MPI
    MPI_Finalize();
    
    return 0;
}
