//
//
#include <iostream>
#include <string>
#include <CmdLineArgParser.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>

int main(int argc, char** argv) {

    CmdLineArgParser args;
    args.setPurpose("Find cell that contains a point.");
    args.set("-m", "cs.vtk", "Mesh file name.");
    args.set("-p", "points.vtk", "File name containing target points.");
    bool success = args.parse(argc, argv);
    bool help = args.get<bool>("-h");

    if (help) {
        args.help();
    }

    if (!success) {
        std::cerr << "ERROR:\nRun " << argv[0] << " -h to see all options\n";
    }


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







    // read the point cloud


}
