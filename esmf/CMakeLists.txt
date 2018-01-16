
include_directories("${CMAKE_SOURCE_DIR}/common" 
	                "${ESMF_DIR}/src/include" 
	                "${ESMF_DIR}/build_config/${ESMF_OS}.${ESMF_COMPILER}.default"
	                "${MPI_DIR}/include"
	                "${ESMF_DIR}/mod/mod${ESMF_BOPT}/${ESMF_OS}.${ESMF_COMPILER}.64.${ESMF_COMM}.default")

link_directories(${CMAKE_BINARY_DIR}/common "${ESMF_DIR}/lib/lib${ESMF_BOPT}/${ESMF_OS}.${ESMF_COMPILER}.64.${ESMF_COMM}.default")

#set(ESMF_LIBRARY "${ESMF_DIR}/lib/lib${ESMF_BOPT}/${ESMF_OS}.${ESMF_COMPILER}.64.${ESMF_COMM}.default/libesmf.a")



add_executable(findCellEsmf findCellEsmf.F90 vtk_reader.cpp)
#target_link_libraries(findCellEsmf CmdLineArgParser "${ESMF_LIBRARY}" gfortran pthread mpifort dl rt)
target_link_libraries(findCellEsmf "${VTK_LIBRARIES}" esmf pthread mpifort dl)

