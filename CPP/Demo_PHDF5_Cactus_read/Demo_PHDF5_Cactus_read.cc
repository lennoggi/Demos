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

    const auto metadata_group_id = H5Gopen(file_id, "Parameters and Global Attributes", H5P_DEFAULT);
    assert(metadata_group_id >= 0);


    // Read 'nioprocs'
    const auto nioprocs_attr_id = H5Aopen(metadata_group_id, "nioprocs", H5P_DEFAULT);
    assert(nioprocs_attr_id >= 0);

    int nioprocs;
    CHECK_ERROR(H5Aread(nioprocs_attr_id, H5T_NATIVE_INT, &nioprocs));
    CHECK_ERROR(H5Aclose(nioprocs_attr_id));
    INFO("nioprocs = " << nioprocs);


    /* Read 'All Parameters'
     * NOTE: see
     *       https://support.hdfgroup.org/ftp/HDF5/examples/examples-by-api/hdf5-examples/1_10/C/H5T/h5ex_t_string.c
     *       to see how to properly read a fixed-size string in HDF5            */
    const auto allpars_dset_id = H5Dopen(metadata_group_id, "All Parameters", H5P_DEFAULT);
    assert(allpars_dset_id >= 0);

    const auto allpars_dspace_id = H5Dget_space(allpars_dset_id);
    assert(allpars_dspace_id >= 0);

    const auto allpars_dspacetype_id = H5Sget_simple_extent_type(allpars_dspace_id);
    CHECK_ERROR(H5Sclose(allpars_dspace_id));
    assert(allpars_dspacetype_id == H5S_SCALAR);

    const auto allpars_dtype_id = H5Dget_type(allpars_dset_id);
    assert(allpars_dtype_id >= 0);

    const auto allpars_dtypeclass = H5Tget_class(allpars_dtype_id);
    assert(allpars_dtypeclass == H5T_STRING);

    const auto allpars_size_bytes = H5Tget_size(allpars_dtype_id);
    CHECK_ERROR(H5Tclose(allpars_dtype_id));
    assert(allpars_size_bytes > 0);
    INFO("'All Parameters' string size = " << allpars_size_bytes);

    const auto allpars_memtype_id = H5Tcopy(H5T_C_S1);
    assert(allpars_memtype_id >= 0);
    CHECK_ERROR(H5Tset_size(allpars_memtype_id, allpars_size_bytes));

    const auto allpars_dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    assert(allpars_dxpl_id >= 0);
    CHECK_ERROR(H5Pset_dxpl_mpio(allpars_dxpl_id, H5FD_MPIO_COLLECTIVE));

    char *buffer = new char[allpars_size_bytes];
    CHECK_ERROR(H5Dread(allpars_dset_id, allpars_memtype_id,
                        H5S_ALL, H5S_ALL, allpars_dxpl_id, buffer));
    CHECK_ERROR(H5Tclose(allpars_memtype_id));
    const string allpars_str(buffer);
    delete [] buffer;
    INFO("All Parameters =\n" << allpars_str);

    CHECK_ERROR(H5Pclose(allpars_dxpl_id));
    CHECK_ERROR(H5Dclose(allpars_dset_id));


    CHECK_ERROR(H5Gclose(metadata_group_id));
    CHECK_ERROR(H5Fclose(file_id));

    CHECK_ERROR(MPI_Finalize());

    return 0;
}
