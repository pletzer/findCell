// Using ESMF to locate the cell
//
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <CmdLineArgParser.h>
#include "ESMC.h"

int main(int argc, char** argv) {

    CmdLineArgParser args;
    args.setPurpose("Find cell that contains a point.");
    args.set("-m", std::string("cs.vtk"), "Mesh file name.");
    args.set("-p", std::string("points.vtk"), "File name containing target points.");
    args.set("-t", 1.e-10, "Tolerance.");
    bool success = args.parse(argc, argv);
    bool help = args.get<bool>("-h");

    if (help) {
        args.help();
        return 0;
    }

    if (!success) {
        std::cerr << "ERROR:\nRun " << argv[0] << " -h to see all options\n";
        return 1;
    }

    // error code
    int rc = ESMF_RC_NOT_IMPL;

    // read the mesha
    const char* filename = args.get<std::string>("-m").c_str();
    std::cout << "Reading mesh and field from file " << filename << '\n';

    // checking the file exists
    std::ifstream infile(filename);
    if (!infile.good()){
        std::cerr << "ERROR:\nMesh file  " << filename << " cannot be opened. Specify mesh file with -m <mesh_file>\n";
        return 2;
    }

    // initialize
    rc = ESMC_Initialize(NULL, ESMC_ArgLast);


    int fileTypeFlag = ESMC_FILEFORMAT_VTK;
    int convertToDual = 0;
    int addUserArea = 0;
    const char* meshname = "";
    int maskFlag = 0;
    const char* varname = "";
    ESMC_Mesh mesh = ESMC_MeshCreateFromFile(filename, fileTypeFlag, &convertToDual, &addUserArea, 
                                             meshname, &maskFlag, varname, &rc);
    if (rc != ESMF_SUCCESS) std::cerr<< "ERROR: after creating mesh, rc = " << rc << '\n';

    //std::cout << "Number of failures     : " << totFailures << " (" << 100.*double(totFailures)/double(numPoints) << " %)\n";
    //std::cout << "Avg time per point  (s): " << elapsed_secs/double(numPoints) << '\n';
    //std::cout << "Time for all points (s): " << elapsed_secs << '\n';

    // clean up
    rc = ESMC_MeshDestroy(&mesh);
    if (rc != ESMF_SUCCESS) std::cerr<< "ERROR: after destroying mesh, rc = " << rc << '\n';
    ESMC_Finalize();
    
    return 0;
}
