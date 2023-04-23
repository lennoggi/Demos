#include <cassert>
#include <iostream>
#include <array>
#include <mpi.h>
#include <hdf5.h>

using namespace std;

#define FILENAME "TestFile.h5"
#define DSETNAME "TestDset"
#define SIZE     100000


#define CHECK_ERROR(routine)                     \
do {                                             \
    const int err = routine;                     \
    if (err < 0) {                               \
        cout << "Routine '" << #routine          \
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
    CHECK_ERROR(MPI_Comm_size(MPI_COMM_WORLD, &nranks));
    CHECK_ERROR(MPI_Comm_rank(MPI_COMM_WORLD, &rankid));

    /* Create a file access property list with parallel I/O access using the
     * world communicator and the null MPI_Info object                          */
    auto fapl_id = H5Pcreate(H5P_FILE_ACCESS);
    assert(fapl_id >= 0);
    CHECK_ERROR(H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL));

    // Collectively create a new file (overwrite if present)
    const auto file_id_create = H5Fcreate(FILENAME, H5F_ACC_TRUNC,
                                          H5P_DEFAULT, fapl_id);
    assert(file_id_create >= 0);


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
    const auto dset_id = H5Dcreate(file_id_create, DSETNAME, H5T_NATIVE_INT, fspace_id,
                                   H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    assert(dset_id >= 0);


    // Create a collective dataset transfer property list
    const auto dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    assert(dxpl_id >= 0);
    CHECK_ERROR(H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE));


    // Generate and write some data to the dataset
    array<int, SIZE> data;

    for (int i = 0; i < SIZE; ++i) {
        data.at(i) = rankid + i;
    }

    CHECK_ERROR(H5Dwrite(dset_id, H5T_NATIVE_INT, mspace_id, fspace_id,
                         dxpl_id, data.data()));


    // Close all HDF5 identifiers that can be closed
    CHECK_ERROR(H5Dclose(dset_id));
    CHECK_ERROR(H5Sclose(mspace_id));
    CHECK_ERROR(H5Sclose(fspace_id));
    CHECK_ERROR(H5Pclose(dxpl_id));
    CHECK_ERROR(H5Fclose(file_id_create));


    // Re-open the file using the same file access property list
/*    const auto file_id_open = H5Fopen(FILENAME, H5F_ACC_RDONLY, fapl_id);
    assert(file_id_open != H5I_INVALID_HID);

    // Close the file
    CHECK_ERROR(H5Fclose(file_id_open));*/

    // Close the file access property list
    CHECK_ERROR(H5Pclose(fapl_id));

    // Finalize MPI
    CHECK_ERROR(MPI_Finalize());
    
    return 0;
}
