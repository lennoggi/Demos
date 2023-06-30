#include <cassert>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int nranks;
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    assert(nranks >= 2);

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    /* MPI_Comm_create only returns a valid new communicator when called by a
     * rank which is included in the new communicator (i.e., ranks 0 and 1 here)*/
    const int newcomm_ranks[2] = {0, 1};
    MPI_Group newgroup;
    MPI_Group_incl(world_group, 2, newcomm_ranks, &newgroup);

    MPI_Comm newcomm;
    MPI_Comm_create(MPI_COMM_WORLD, newgroup, &newcomm);


    int rankid;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankid);

    /* The new communicator can only be used if valid (i.e., it can only be used
     * by ranks 0 and 1 here)                                                   */
    //if (rankid == 0 or rankid == 1) {
    if (newcomm != MPI_COMM_NULL) {
        int newcomm_size;
        MPI_Comm_size(newcomm, &newcomm_size);
        std::cout << "[Rank " << rankid << "] Size of the new communicator: " << newcomm_size << std::endl;
    }

    MPI_Finalize();
    return 0;
}
