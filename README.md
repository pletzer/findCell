# findCell

Comparing different libraries for finding the unstructured grid cell that contains a point

## How to build

You will need python 2.7 and the libraries ESMF (tested with 7.0.2) and VTK (tested with 8.0.1) built. To generate the input files, you will need to have VTK built with python enabled. 

You'll need to set:

 * ```fortran_compiler_id```: currently either "Intel" or "GNU". This determines the Fortran libraries to link the C++ executables against. 
 * ```ESMF_DIR```: the top directory where ESMF was built
 * ```ESMF_OS```: either "Linux" or "Darwin". Determines the location of the ESMF library.
 * ```ESMF_COMM```: either "mpich" or "openmpi". Determines the location of the ESMF library.
 * ```MPI_DIR```: the MPI libraries are assumed to be located under $MPI_DIR/lib
 * ```VTK_DIR```: the path to UseVTK.cmake, typically under /vtk_top_dir/lib/cmake/vtk-version/

To compile, type:

 * ```git clone https://github.com/pletzer/findCell```
 * ```cd findCell; mkdir build; cd build```
 * ```
 cmake -DFORTRAN_COMPILER_ID=$fortran_compiler_id \
 -DESMF_DIR=$ESMF_DIR -DESMF_OS=$ESMF_OS -DESMF_COMM=$ESMF_COMM \
 -DMPI_DIR=$MPI_DIR -DVTK_DIR=$vtk_dir ..
 ```
 * ```make```


## How to run

```cd ../input_data``` and run ```sh generateCubedSphereMesh.py```. This will generate cubed-sphere 3D meshes of various resolutions and save the meshes in VTK files (cs*.vtk). 

```cd ../build``` and run the tests: ```sh input_data/runFindCell.sh > results.csv``` (this may take up to a few hours). You can plot the results with ```python plotResults.py -i results.csv```.

## Results



