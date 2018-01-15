cmake_minimum_required (VERSION 3.3)
project(findCell)

enable_testing()

set(ESMF_DIR "NOT-FOUND" CACHE PATH "Top directory where ESMF was built")
set(ESMF_OS "Linux" CACHE STRING "ESMF target OS (eg Linux or Unicos)")
set(ESMF_COMPILER "gfortran" CACHE STRING "ESMF compiler (eg gfortran or intel)")
set(ESMF_COMM "mpich" CACHE STRING "ESMF communicator type (eg mpich or openmpi)")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

message(STATUS "==============================================================")
message(STATUS "C++     compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "           flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "==============================================================")

add_subdirectory(common)
add_subdirectory(vtk)
add_subdirectory(esmf)

