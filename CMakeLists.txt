# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# set CMake project name
project(TEST)

# install(CODE) uses generator expressions - requires 3.14 or newer.
# We want it to be on, but until we can bump our minimum requirement
# set it to OLD to make sure we don't break anything.
if (POLICY CMP0087)
  cmake_policy(SET CMP0087 OLD)
endif (POLICY CMP0087)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include(Path_Setup)
include(RPath_Setup)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
relative_rpath(RELPATH)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_DIR}${RELPATH}")
std_build_rpath()

set(CMAKE_INSTALL_PREFIX "/usr/test/")

include(ExternalProject)
include(ExternalProject_Target)

add_definitions(-g)

include("${CMAKE_CURRENT_SOURCE_DIR}/netpbm.cmake")

include_directories("${CMAKE_BINARY_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}/include/netpbm")
add_executable(test main.c)
target_link_libraries(test netpbm)
add_dependencies(test netpbm_stage)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/ppm.stamp
  COMMAND test ${CMAKE_CURRENT_SOURCE_DIR}/m35.ppm
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/ppm.stamp
  DEPENDS test
  )
add_custom_target(ppmtest ALL DEPENDS ${CMAKE_BINARY_DIR}/ppm.stamp)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8

