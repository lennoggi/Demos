#ifndef HDF5_MPIO_HH
#define HDF5_MPIO_HH

#include <string>
#include <unordered_map>
#include <hdf5.h>


// A map linking HDF5 actual I/O modes to appropriate descriptive strings
const std::unordered_map<H5D_mpio_actual_io_mode_t, std::string> H5D_mpio_actual_io_mode_map {
    {H5D_MPIO_NO_COLLECTIVE,         "H5D_MPIO_NO_COLLECTIVE"},
    {H5D_MPIO_CHUNK_INDEPENDENT,     "H5D_MPIO_CHUNK_INDEPENDENT"},
    {H5D_MPIO_CHUNK_COLLECTIVE,      "H5D_MPIO_CHUNK_COLLECTIVE"},
    {H5D_MPIO_CHUNK_MIXED,           "H5D_MPIO_CHUNK_MIXED"},
    {H5D_MPIO_CONTIGUOUS_COLLECTIVE, "H5D_MPIO_CONTIGUOUS_COLLECTIVE"}
};

/* A map linking HDF5 actual chunk optimization modes to appropriate descriptive
 * strings                                                                      */
const std::unordered_map<H5D_mpio_actual_chunk_opt_mode_t, std::string> H5D_mpio_actual_chunk_opt_mode_map {
    {H5D_MPIO_NO_CHUNK_OPTIMIZATION, "H5D_MPIO_NO_CHUNK_OPTIMIZATION"},
    {H5D_MPIO_LINK_CHUNK,            "H5D_MPIO_LINK_CHUNK"},
    {H5D_MPIO_MULTI_CHUNK,           "H5D_MPIO_MULTI_CHUNK"}
};

/* A map linking HDF5 non-collective I/O mode causes to appropriate descriptive
 * strings                                                                      */
const std::unordered_map<uint32_t, std::string> H5Pget_mpio_no_collective_cause_map {
    {H5D_MPIO_COLLECTIVE,                               "H5D_MPIO_COLLECTIVE"},
    {H5D_MPIO_SET_INDEPENDENT,                          "H5D_MPIO_SET_INDEPENDENT"},
    {H5D_MPIO_DATATYPE_CONVERSION,                      "H5D_MPIO_DATATYPE_CONVERSION"},
    {H5D_MPIO_DATA_TRANSFORMS,                          "H5D_MPIO_DATA_TRANSFORMS"},
    {H5D_MPIO_MPI_OPT_TYPES_ENV_VAR_DISABLED,           "H5D_MPIO_MPI_OPT_TYPES_ENV_VAR_DISABLED"},
    {H5D_MPIO_NOT_SIMPLE_OR_SCALAR_DATASPACES,          "H5D_MPIO_NOT_SIMPLE_OR_SCALAR_DATASPACES"},
    {H5D_MPIO_NOT_CONTIGUOUS_OR_CHUNKED_DATASET,        "H5D_MPIO_NOT_CONTIGUOUS_OR_CHUNKED_DATASET"},
    {H5D_MPIO_PARALLEL_FILTERED_WRITES_DISABLED,        "H5D_MPIO_PARALLEL_FILTERED_WRITES_DISABLED"},
    {H5D_MPIO_ERROR_WHILE_CHECKING_COLLECTIVE_POSSIBLE, "H5D_MPIO_ERROR_WHILE_CHECKING_COLLECTIVE_POSSIBLE"},
    {H5D_MPIO_NO_COLLECTIVE_MAX_CAUSE,                  "H5D_MPIO_NO_COLLECTIVE_MAX_CAUSE"}
};


#endif  // HDF5_MPIO_HH
