cmake_minimum_required (VERSION 3.3)
project(findCell)

enable_testing()

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

message(STATUS "==============================================================")
message(STATUS "C++     compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "           flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "==============================================================")

add_subdirectory(common)
add_subdirectory(vtk)
