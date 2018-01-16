#include <iostream>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkIdList.h>

#ifndef VTK_READER
#define VTK_READER

extern "C" {

    void vtk_reader_new(vtkUnstructuredGridReader** self);
    void vtk_reader_setfilename(vtkUnstructuredGridReader** self, const char* filename, int* len, ...);
    void vtk_reader_getnumberofpoints(vtkUnstructuredGridReader** self, int* n);
    void vtk_reader_fillvertices(vtkUnstructuredGridReader** self, int* pointIds, double* xyz);
    void vtk_reader_fillconnectivity(vtkUnstructuredGridReader** self, int* elementIds, int* elementConn);
    void vtk_reader_del(vtkUnstructuredGridReader** self);

}

#endif // VTK_READER
