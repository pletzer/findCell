
include_directories(${CMAKE_SOURCE_DIR}/common)
link_directories(${CMAKE_BINARY_DIR}/common)

add_executable(findCellVtk findCellVtk.cxx)
target_link_libraries(findCellVtk CmdLineArgParser ${VTK_LIBRARIES})

add_executable(findCellVtkCellLocator findCellVtkCellLocator.cxx)
target_link_libraries(findCellVtkCellLocator CmdLineArgParser ${VTK_LIBRARIES})

