
include_directories("${CMAKE_SOURCE_DIR}/common" 
	                "${ESMF_DIR}/src/include" 
	                "${ESMF_DIR}/build_config/${ESMF_OS}.${ESMF_COMPILER}.default"
	                "${MPI_DIR}/include"
	                "${ESMF_DIR}/mod/mod${ESMF_BOPT}/${ESMF_OS}.${ESMF_COMPILER}.64.${ESMF_COMM}.default")

link_directories(${CMAKE_BINARY_DIR}/common "${ESMF_DIR}/lib/lib${ESMF_BOPT}/${ESMF_OS}.${ESMF_COMPILER}.64.${ESMF_COMM}.default")

message(STATUS "FORTRAN_COMPILER_ID is ${FORTRAN_COMPILER_ID}")
set(XTRA_LIBS "")
if (FORTRAN_COMPILER_ID STREQUAL "Intel")
  set(XTRA_LIBS "mpifort")
elseif (FORTRAN_COMPILER_ID STREQUAL "GNU")
  set(XTRA_LIBS "mpi_mpifh")
endif()
message(STATUS "Adding ${XTRA_LIBS} to linker")
add_executable(findCellEsmf findCellEsmf.F90 vtk_reader.cpp)
target_link_libraries(findCellEsmf "${VTK_LIBRARIES}" esmf pthread ${XTRA_LIBS} dl)

