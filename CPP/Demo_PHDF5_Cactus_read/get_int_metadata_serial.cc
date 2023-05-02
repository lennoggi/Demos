#include <cassert>
#include <iostream>
#include <string>

#include <hdf5.h>

using namespace std;


#define CHECK_ERROR(routine)                              \
do {                                                      \
    const int err = routine;                              \
    if (err < 0) {                                        \
        cout << "Routine '" << #routine <<                \
                "' returned error code " << err << endl;  \
        return -1;                                        \
    }                                                     \
} while (0)


#define INPUT_FILE "/scratch3/07825/lennoggi/CBD_handoff_IGM_McLachlan_Spinning_aligned08/output-0000/HDF5_3D/rho_b.xyz.file_0.h5"
#define ATTR_NAME  "nioprocs"



int main(int argc, char **argv) {
    const auto file_id = H5Fopen(INPUT_FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
    assert(file_id >= 0);

    const auto group_id = H5Gopen(file_id, "Parameters and Global Attributes", H5P_DEFAULT);
    assert(group_id >= 0);

    const auto attr_id = H5Aopen(group_id, ATTR_NAME, H5P_DEFAULT);
    assert(attr_id >= 0);

    const auto dspace_id = H5Aget_space(attr_id);
    assert(dspace_id >= 0);


    const auto ndims = H5Sget_simple_extent_ndims(dspace_id);

    /*if (ndims != 1) {
        const string attr_str = ATTR_NAME;
        cout << "Attribute '" << attr_str <<
                "' is not 1-dimensional (ndims = " << ndims << ")" << endl;
        return -1;
    }*/


    int attr_value;
    CHECK_ERROR(H5Aread(attr_id, H5T_NATIVE_INT, &attr_value));
    cout << attr_value << endl;

    CHECK_ERROR(H5Aclose(attr_id));
    CHECK_ERROR(H5Sclose(dspace_id));
    CHECK_ERROR(H5Gclose(group_id));
    CHECK_ERROR(H5Fclose(file_id));

    return 0;
}
