#include <array>
#include <openPMD/openPMD.hpp>

#define FILENAME "testfile.h5"

#define NX 10
#define NY 20



using namespace std;
namespace io = openPMD;


int main() {
    // Create a file and a series in that file
    auto series = io::Series(FILENAME, io::Access::CREATE);

    // Create an iteration within the series
    auto it = series.iterations[0];

    // Create a record within the iteration
    auto field = it.meshes["Some physical field"];
    auto fx    = field["x"];
    auto fy    = field["y"];
    auto fz    = field["z"];


    /* Generate some mock data as a 1D array which is meant to represent data
     * on a 2D mesh                                                             */
    array<double, NX*NY> xdata, ydata, zdata;
    int count = 0;

    for (auto i = decltype(NX){0}; i < NX; ++i) {
        const auto i_idx = i*NY;
        for (auto j = decltype(NY){0}; j < NY; ++j) {
            xdata.at(i_idx + j) = count + 0.01;
            ydata.at(i_idx + j) = count + 0.02;
            zdata.at(i_idx + j) = count + 0.03;
            ++count;
        }
    }


    /* Create a 2D dataset with shape {NX, NY} and assign it to the field's
     * components                                                               */
    auto dataset = io::Dataset(io::determineDatatype<double>(), {NX, NY});

    fx.resetDataset(dataset);
    fy.resetDataset(dataset);
    fz.resetDataset(dataset);


    /* Register the data to be written to file. The idea is that different
     * chunks of data can be in principle registered at different times, so we
     * do all data registration first and only then write ('flush') to file.    */
    fx.storeChunk(xdata, {0, 0}, {NX, NY});
    fy.storeChunk(ydata, {0, 0}, {NX, NY});
    fz.storeChunk(zdata, {0, 0}, {NX, NY});

    // Flush
    series.flush();

    return 0;
}
