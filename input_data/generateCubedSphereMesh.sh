#!/usr/bin/bash 

for n in 5 10 20 50 100 200 500 1000; do
  echo $n
  python generateCubedSphereMesh.py -n $n -z $n -r 0.1 -o cs${n}.vtk
done
