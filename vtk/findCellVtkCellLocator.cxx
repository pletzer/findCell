// Using the standard FinCell method  that comes with vtkUnstructuredGrid
//
#include <iostream>
#include <string>
#include <ctime>
#include <CmdLineArgParser.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellLocator.h>
#include <vtkGenericCell.h>

int main(int argc, char** argv) {

    CmdLineArgParser args;
    args.setPurpose("Find cell that contains a point.");
    args.set("-m", std::string("cs.vtk"), "Mesh file name.");
    args.set("-p", std::string("points.vtk"), "File name containing target points.");
    args.set("-t", 1.e-10, "Tolerance.");
    args.set("-b", 100, "Average number of cells per bucket in cell locator.");
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

    // read the mesh
    vtkUnstructuredGridReader* meshReader = vtkUnstructuredGridReader::New();
    meshReader->SetFileName(args.get<std::string>("-m").c_str());
    meshReader->Update();
    vtkUnstructuredGrid* mesh = meshReader->GetOutput();
    std::cout << "Mesh file: " << args.get<std::string>("-m") << " no cells: " << mesh->GetNumberOfCells() << '\n';

    // read the points
    vtkUnstructuredGridReader* pointReader = vtkUnstructuredGridReader::New();
    pointReader->SetFileName(args.get<std::string>("-p").c_str());
    pointReader->Update();
    vtkUnstructuredGrid* points = pointReader->GetOutput();
    std::cout << "Point file: " << args.get<std::string>("-p") << " no points: " << points->GetNumberOfPoints() << '\n';

    // build the cell locator
    vtkCellLocator* loc = vtkCellLocator::New();
    loc->SetDataSet(mesh);
    loc->SetNumberOfCellsPerBucket(args.get<int>("-b"));
    loc->BuildLocator();

    // iterate over each point
    double xyz[3];
    const double tol2 = args.get<double>("-t");
    vtkGenericCell* genCell = vtkGenericCell::New();
    double pcoords[3];
    double weights[8];
    int totFailures = 0;
    size_t numPoints = points->GetNumberOfPoints();
    std::clock_t tic = clock();
    for (size_t i = 0; i < numPoints; ++i) {
        points->GetPoints()->GetPoint(i, xyz);
        vtkIdType cellId = loc->FindCell(xyz, tol2, genCell, pcoords, weights);
        if (cellId < 0) totFailures++;
    }
    std::clock_t toc = clock();
    double elapsed_secs = double(toc - tic) / CLOCKS_PER_SEC;

    std::cout << "Number of failures     : " << totFailures << " (" << 100.*double(totFailures)/double(numPoints) << " %)\n";
    std::cout << "Avg time per point  (s): " << elapsed_secs/double(numPoints) << '\n';
    std::cout << "Time for all points (s): " << elapsed_secs << '\n';

    // clean up
    genCell->Delete();
    loc->Delete();
    pointReader->Delete();
    meshReader->Delete();

    return 0;
}
