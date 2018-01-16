#include  "vtk_reader.h"
#include <string>

extern "C" {

void vtk_reader_new_(vtkUnstructuredGridReader** self) {
    *self = vtkUnstructuredGridReader::New();
}

void vtk_reader_setfilename_(vtkUnstructuredGridReader** self, 
                             const char* filename, int* len, ...) {
    // add termination character
    std::string fn = std::string(filename, 0, *len);
    fn += std::string("\0");
    (*self)->SetFileName(fn.c_str());
    // read the file
    (*self)->Update();
}

void vtk_reader_getnumberofpoints_(vtkUnstructuredGridReader** self, 
                                   int* n) {
    *n = (*self)->GetOutput()->GetNumberOfPoints();
}

void vtk_reader_getnumberofcells_(vtkUnstructuredGridReader** self, 
                                  int* n) {
    *n = (*self)->GetOutput()->GetNumberOfCells();
}

void vtk_reader_fillvertices_(vtkUnstructuredGridReader** self, 
                              int* pointIds, double* xyz) {
    vtkPoints* points = (*self)->GetOutput()->GetPoints();
    vtkIdType numPoints = points->GetNumberOfPoints();
    for (vtkIdType i = 0; i < numPoints; ++i) {
        pointIds[i] = i + 1; // indexing is 1-based in ESMF
        points->GetPoint(i, &xyz[3*i]);
    }
}

void vtk_reader_fillconnectivity_(vtkUnstructuredGridReader** self, 
                                  int* elementIds, int* elementConn) {
    vtkCellArray* cells = (*self)->GetOutput()->GetCells();
    vtkIdType index = 0;
    vtkIdList* ptIds = vtkIdList::New();
    cells->InitTraversal();
    for (vtkIdType cellId = 0; cellId < cells->GetNumberOfCells(); ++cellId) {
        elementIds[cellId] = cellId + 1;
        cells->GetNextCell(ptIds);
        vtkIdType numVerts = ptIds->GetNumberOfIds();
        for (vtkIdType k = 0; k < numVerts; ++k) {
            elementConn[index + k] = ptIds->GetId(k) + 1; // index is 1-based
        }
        index += numVerts;
    }
}


void vtk_reader_del_(vtkUnstructuredGridReader** self) {
    (*self)->Delete();
}

}