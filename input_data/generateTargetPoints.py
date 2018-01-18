import random
import argparse
import numpy

parser = argparse.ArgumentParser(description='Generate random points')
parser.add_argument('-N', type=int, default=100, help='Number of points')
parser.add_argument('-a', type=float, default=1.0, help='Radius')
parser.add_argument('-r', type=float, default=0.1, help='Max relative elevation')
parser.add_argument('-o', default="points.vtk", help='Output VTK file')

args = parser.parse_args()

n = args.N
maxRelElv = args.r
radius = args.a

numpy.random.seed(1234)
lams = numpy.random.rand(n) * 2*numpy.pi
thes = -numpy.pi/2. + numpy.pi*numpy.random.rand(n)
elvs = radius*(maxRelElv*numpy.random.rand(n) + 1.0)
xyz = numpy.zeros((n, 3), numpy.float64)
xyz[:, 0] = elvs*numpy.cos(thes)*numpy.cos(lams)
xyz[:, 1] = elvs*numpy.cos(thes)*numpy.cos(lams)
xyz[:, 2] = elvs*numpy.sin(thes)

# save the points as a set of x,y,z points
import vtk
mesh = vtk.vtkUnstructuredGrid()
pts = vtk.vtkPoints()
ptdata = vtk.vtkDoubleArray()

ptdata.SetNumberOfTuples(n)
ptdata.SetNumberOfComponents(3) # 3d
ptdata.SetVoidArray(xyz, 3*n, 1)

pts.SetNumberOfPoints(n)
pts.SetData(ptdata)

mesh.SetPoints(pts)
#no cell

writer = vtk.vtkUnstructuredGridWriter()
writer.SetFileName('points.vtk')
writer.SetInputData(mesh)
writer.Update()
