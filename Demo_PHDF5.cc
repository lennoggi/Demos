#include <cassert>
#include <mpi.h>
#include <hdf5.h>

#define FILENAME "TestFile.h5"

#define H5_CHECK_ERROR(h5call)                               \
do {                                                  \
    const int err = h5call;                           \
    if (err < 0) {                                    \
        std::cout << "HDF5 call '" << #h5call         \
                  << "' returned error code " << err  \
                  << std::endl;                       \
    }                                                 \
} while (0)


int main(int argc, char **argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the total number of ranks and the ID of the current rank
    /*int n_ranks, rank_id;
    MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);*/

    /* Create a file access property list with parallel I/O access using the
     * world communicator and the null MPI_Info object                          */
    auto fapl_id = H5Pcreate(H5P_FILE_ACCESS);
    assert(fapl_id != H5I_INVALID_HID);
    H5_CHECK_ERROR(H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL));

    // Collectively create a new file (overwrite if present)
    const auto file_id_create = H5Fcreate(FILENAME, H5F_ACC_TRUNC,
                                          H5P_DEFAULT, fapl_id);
    assert(file_id_create != H5I_INVALID_HID);

    // Close the file
    H5_CHECK_ERROR(H5Fclose(file_id_create));

    // Re-open the file using the same file access property list
    const auto file_id_open = H5Fopen(FILENAME, H5F_ACC_RDONLY, fapl_id);
    assert(file_id_open != H5I_INVALID_HID);

    // Close the file
    H5_CHECK_ERROR(H5Fclose(file_id_open));

    // Close the file access property list
    H5_CHECK_ERROR(H5Pclose(fapl_id));

    // Finalize MPI
    MPI_Finalize();
    
    return 0;
}
