from igCubedSphereElv import CubedSphereElv
import argparse

parser = argparse.ArgumentParser(description='Generate cubed sphere grid')
parser.add_argument('-n', type=int, default=10, help='Number of cells along each direction of the tiles')
parser.add_argument('-z', type=int, default=2, help='Number of vertical cells')
args = parser.parse_args()

numCells = args.n
numElvs = args.z

cs = CubedSphereElv(numCells, numElvs=numElvs)
cs.save('cs.vtk')