#include  "vtk_reader.h"
#include <string>

extern "C" {

void vtk_reader_new_(vtkUnstructuredGridReader** self) {
    *self = vtkUnstructuredGridReader::New();
}

void vtk_reader_setfilename_(vtkUnstructuredGridReader** self, const char* filename, int len, ...) {
    // add termination character
    std::string fn = std::string(filename).substr(0, len) + std::string('\0');
    (*self)->SetFileName(fn.c_str());
    // read the file
    (*self)->Update();
}

void vtk_reader_getnumberofpoints_(vtkUnstructuredGridReader** self, int* n) {
    *n = (*self)->GetOutput()->GetNumberOfPoints();
}

void vtk_reader_fillvertices_(vtkUnstructuredGridReader** self, int* pointIds, double* xyz) {
    for (size_t i = 0; i < (*self)->GetOutput()->GetNumberOfPoints(); ++i) {
        pointIds[i] = i + 1;
        (*self)->GetOutput()->GetPoints()->GetPoint(i, &xyz[3*i]);
    }
}

void vtk_reader_del_(vtkUnstructuredGridReader** self) {
    (*self)->Delete();
}

}