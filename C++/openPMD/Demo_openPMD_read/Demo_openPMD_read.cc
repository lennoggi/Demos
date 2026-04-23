#include <iostream>
#include <openPMD/openPMD.hpp>

#define FILENAME "/shared/rc/ccrg/le8016/Cylindrical_blast_paper/output-0000/Cylindrical_blast/Cylindrical_blast.it00000150.bp"
//#define FILENAME "/shared/rc/ccrg/le8016/Cylindrical_blast_paper/output-0000/Cylindrical_blast/Cylindrical_blast.it%T.%E"  // Wildcards %T and %E

using namespace std;
namespace io = openPMD;


int main() {
    // Access the series in the file
    auto series = io::Series(FILENAME, io::Access::READ_ONLY);


    // List all the available series attributes
    cout << endl << "Available series attributes:"
         << endl << "============================" << endl;
    for (const auto &attr : series.attributes()) {
        cout << attr << endl;
    }

    cout << endl << "Series attributes:"
         << endl << "=================="
         << endl << "Author:             " << series.author()
         << endl << "Base path:          " << series.basePath()
         << endl << "Date:               " << series.date()
         << endl << "Iteration encoding: " << series.iterationEncoding()
         << endl << "Iteration format:   " << series.iterationFormat()
         << endl << "Machine:            " << series.machine()
         << endl << "Meshes path:        " << series.meshesPath()
         << endl << "openPMD version:    " << series.openPMD()
         << endl << "openPMD extension:  " << series.openPMDextension()
         << endl << "Software:           " << series.software()
         << endl << "Software version:   " << series.softwareVersion();


    // List all the available iterations
    cout << endl << endl << endl
         << "The series contains " << series.iterations.size() << " iterations"  << endl;


    cout << endl << "Available iterations:"
         << endl << "=====================" << endl;
    /* series.iterations is a map from integers (the iteration number) to
     * openPMD Iteration objects storing all the info about the iteration.
     * However, using it.second (see below) is probably not the recommended way
     * to access the iteration data.                                            */
    for (const auto &it : series.iterations) {
        cout << "Iteration " << it.first << endl;
    }

    cout << endl << "Attributes of the iterations as a whole:"
         << endl << "========================================" << endl;
    for (const auto &attr : series.iterations.attributes()) {
        cout << attr << endl;
    }

    cout << endl << "Details of each iteration:"
         << endl << "==========================" << endl;
    //for (const auto &it : series.readIterations()) {  // This forbids using it.close() (see end of file)
    for (auto it : series.readIterations()) {
        cout << "Iteration " << it.iterationIndex << ":" << endl
             << "  Available attributes:" << endl
             << "  ---------------------" << endl;
        for (const auto &attr : it.attributes()) {
            cout << "  " << attr << endl;
        }

        cout << "  -----"
             // XXX: all these are listed as attributes above, but they don't seem to exist
             //<< endl << "  All parameters:          " << it.AllParameters<string>()
             //<< endl << "  chunkInfo_patch00_lev00: " << it.chunkInfo_patch00_lev00<double>()
             //<< endl << "  numDims:                 " << it.numDims<int>()
             //<< endl << "  numLevels_patch00:       " << it.numLevels_patch00<int>()
             //<< endl << "  numPatches:              " << it.numPatches<int>()
             //<< endl << "  patchSuffixes:           " << it.patchSuffixes<int>()
             << endl << "  Time: " << it.time<double>()
             << endl << "  dt:   " << it.dt<double>() << endl << endl;
             //<< endl << "  timeUnitSI:              " << it.timeUnitSI<double>();

        cout << "  Attributes of mesh data as a whole:" << endl
             << "  -----------------------------------" << endl;
        for (const auto &attr : it.meshes.attributes()) {
            cout << attr << endl;
        }

        cout << endl << "  Mesh data:"
             << endl << "  ----------";
        for (const auto &[m, mclass] : it.meshes) {
            cout << endl << "  " << m
                 << endl << "    Available attributes:" << endl;
            for (auto const &attr : mclass.attributes()) {
                cout << "      " << attr << endl;
            }

            cout << "      -----";

            cout << endl << "      axisLabels:      ";
            for (const auto &label : mclass.axisLabels()) {
                cout << " " << label;
            }

            cout << endl << "      dataOrder:        " << mclass.dataOrder()
                 << endl << "      Geometry:         " << mclass.geometry()
                 << endl << "      gridGlobalOffset:";
            for (const auto &offset : mclass.gridGlobalOffset()) {
                cout << " " << offset;
            }

            cout << endl << "      gridSpacing:     ";
            for (const auto &delta : mclass.gridSpacing<double>()) {
                cout << " " << delta;
            }

            cout << endl << "      gridUnitSI:       " << mclass.gridUnitSI()
                 << endl << "      timeOffset:       " << mclass.timeOffset<double>()
                 << endl << "      unitDimension:   ";
            for (const auto &unit : mclass.unitDimension()) {
                cout << " " << unit;
            }


            // Read mesh records components from the current mesh object
            cout << endl << endl << "    Available record components:" << endl;
            for (const auto &[rc, rcclass] : mclass) {
                cout << "      " << rc
                     << endl << "        Available attributes:" << endl;
                for (const auto &attr : rcclass.attributes()) {
                    cout << "          " << attr << endl;
                }

                cout << "          -----" << endl
                     << "          Position:";
                for (const auto &pos : rcclass.position<double>()) {
                    cout << " " << pos;
                }

                cout << endl << "          unitSI:   " << rcclass.unitSI() << endl;
            }
        }

        it.close();
    }


    return 0;
}
