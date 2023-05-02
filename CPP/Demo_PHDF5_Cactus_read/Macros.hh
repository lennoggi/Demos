#ifndef MACROS_HH
#define MACROS_HH

#include <iostream>  // So that std::cout and std::endl       work  when the macros below are used
#include <sstream>   // So that 'msg_ss.str()'                works when the macros below are used
#include <mpi.h>     // So that MPI_Barrier() and MPI_abort() work  when the macros below are used


// Macro printing an informative message
#define INFO(msg_ss)                                         \
    do {                                                     \
        int proc_ID;                                         \
        MPI_Comm_rank(MPI_COMM_WORLD, &proc_ID);             \
                                                             \
        if (proc_ID == 0) {                                  \
            std::ostringstream ss;                           \
            ss << msg_ss;                                    \
            std::cout << "INFO: " << ss.str() << std::endl;  \
        }                                                    \
    }  while(0)


// Macro printing a warning message
#define WARNING(msg_ss)                                         \
    do {                                                        \
        int proc_ID;                                            \
        MPI_Comm_rank(MPI_COMM_WORLD, &proc_ID);                \
                                                                \
        if (proc_ID == 0) {                                     \
            std::ostringstream ss;                              \
            ss << msg_ss;                                       \
            std::cout << "WARNING: " << ss.str() << std::endl;  \
        }                                                       \
    }  while(0)


// Macro printing an error message and terminating program execution
#define ERROR(msg_ss)                                               \
    do {                                                            \
        int proc_ID;                                                \
        MPI_Comm_rank(MPI_COMM_WORLD, &proc_ID);                    \
                                                                    \
        if (proc_ID == 0) {                                         \
            std::ostringstream ss;                                  \
            ss << msg_ss;                                           \
            std::cerr << "ERROR (file " << __FILE__                 \
                      << ", line " << __LINE__ << ")" << std::endl  \
                      << "  -> " << ss.str() << std::endl;          \
        }                                                           \
                                                                    \
        MPI_Barrier(MPI_COMM_WORLD);                                \
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_INTERN);                  \
    }  while(0)


// Macro checking for errors coming from a routine
#define CHECK_ERROR(routine)                     \
do {                                             \
    const int err = routine;                     \
    if (err < 0) {                               \
        ERROR("Routine '" << #routine <<         \
              "' returned error code " << err);  \
    }                                            \
} while (0)


#endif  // MACROS_HH
