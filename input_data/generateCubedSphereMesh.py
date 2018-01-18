from igCubedSphereElv import CubedSphereElv
import argparse

parser = argparse.ArgumentParser(description='Generate cubed sphere grid')
parser.add_argument('-n', type=int, default=10, help='Number of cells along each direction of the tiles')
parser.add_argument('-z', type=int, default=2, help='Number of vertical cells')
parser.add_argument('-a', type=float, default=1.0, help='Radius')
parser.add_argument('-r', type=float, default=0.1, help='Max relative elevation')
parser.add_argument('-o', default="cs.vtk", help='Output VTK file')

args = parser.parse_args()

numCells = args.n
numElvs = args.z

cs = CubedSphereElv(numCells, numElvs=numElvs, radius=args.a, maxRelElv=args.r)
cs.save(args.o)