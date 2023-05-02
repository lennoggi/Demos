#include <cassert>
#include <iostream>
#include <string>

#include <mpi.h>
#include <hdf5.h>

#include "H5_mpio.hh"
#include "Macros.hh"

using namespace std;

#define INPUT_FILE "/scratch3/07825/lennoggi/CBD_handoff_IGM_McLachlan_Spinning_aligned08/output-0000/HDF5_3D/rho_b.xyz.file_0.h5"


int main(int argc, char **argv) {
    CHECK_ERROR(MPI_Init(&argc, &argv));

    auto fapl_id = H5Pcreate(H5P_FILE_ACCESS);
    assert(fapl_id >= 0);
    CHECK_ERROR(H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL));
    CHECK_ERROR(H5Pset_all_coll_metadata_ops(fapl_id, true));

    const auto file_id = H5Fopen(INPUT_FILE, H5F_ACC_RDONLY, fapl_id);
    assert(file_id >= 0);

    const auto group_id = H5Gopen(file_id, "Parameters and Global Attributes", H5P_DEFAULT);
    assert(group_id >= 0);

    const auto attr_id = H5Aopen(group_id, "nioprocs", H5P_DEFAULT);
    assert(attr_id >= 0);

    int nioprocs;
    CHECK_ERROR(H5Aread(attr_id, H5T_NATIVE_INT, &nioprocs));
    CHECK_ERROR(H5Aclose(attr_id));
    INFO("nioprocs = " << nioprocs);

    CHECK_ERROR(H5Gclose(group_id));
    CHECK_ERROR(H5Fclose(file_id));

    CHECK_ERROR(MPI_Finalize());

    return 0;
}
