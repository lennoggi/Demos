#include <AMReX.H>
#include <AMReX_Print.H>

int main(int argc, char **argv) {
    amrex::Initialize(argc, argv);
    amrex::Print() << "Hello from AMReX version " << amrex::Version() << "\n";
    amrex::Finalize();
}
