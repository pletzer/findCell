//
//
#include <iostream>
#include <string>
#include <ctime>
#include <CmdLineArgParser.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>

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
    std::cout << "Mesh file: " << args.get<std::string>("-m") << '\n';
    std::cout << "Point file: " << args.get<std::string>("-p") << '\n';


    // read the mesh
    vtkUnstructuredGridReader* meshReader = vtkUnstructuredGridReader::New();
    meshReader->SetFileName(args.get<std::string>("-m").c_str());
    meshReader->Update();
    vtkUnstructuredGrid* mesh = meshReader->GetOutput();

    // read the points
    vtkUnstructuredGridReader* pointReader = vtkUnstructuredGridReader::New();
    pointReader->SetFileName(args.get<std::string>("-p").c_str());
    pointReader->Update();
    vtkUnstructuredGrid* points = pointReader->GetOutput();

    // iterate over each point
    double xyz[3];
    const double tol2 = args.get<double>("-t");
    int subId;
    double pcoords[3];
    double weights[8];
    int totFailures = 0;
    std::clock_t tic = clock();
    for (size_t i = 0; i < points->GetNumberOfPoints(); ++i) {
        points->GetPoints()->GetPoint(i, xyz);
        vtkIdType cellId = mesh->FindCell(xyz, NULL, 0, tol2, subId, pcoords, weights);
        if (cellId < 0) totFailures++;
    }
    std::clock_t toc = clock();
    double elapsed_secs = double(toc - tic) / CLOCKS_PER_SEC;

    std::cout << "Number of failures: " << totFailures << '\n';
    std::cout << "Elapsed time      : " << elapsed_secs << '\n';

    return 0;
}
