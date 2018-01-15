
include_directories("${CMAKE_SOURCE_DIR}/common" "${ESMF_DIR}/src/include" "${ESMF_DIR}/build_config/${ESMF_OS}.${ESMF_COMPILER}.default")
link_directories(${CMAKE_BINARY_DIR}/common)

set(ESMF_LIBRARY "${ESMF_DIR}/lib/libO/${ESMF_OS}.${ESMF_COMPILER}.64.${ESMF_COMM}.default/libesmf.a")

add_executable(findCellEsmf findCellEsmf.cxx)
target_link_libraries(findCellEsmf CmdLineArgParser "${ESMF_LIBRARY}" -lgfortran -lpthread -lmpifort -dl -lrt)

