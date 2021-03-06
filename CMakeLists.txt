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

# 3rd party build outputs use a CMAKE_RPATH_BUILD setting that is different
# from the "vanilla" management used with other BRL-CAD targets.  The
# function below encapsulates the setup needed for CMAKE_RPATH_BUILD
ext_build_rpath()


set(CMAKE_INSTALL_PREFIX "/usr/test/")

include(ExternalProject)
include(ExternalProject_Target)
add_definitions(-g)

include("${CMAKE_CURRENT_SOURCE_DIR}/stepcode.cmake")

add_custom_target(otool_run ALL
  COMMAND otool -l ${CMAKE_BINARY_DIR}/bin/exp2cxx
  COMMAND otool -l ${CMAKE_BINARY_DIR}/lib/libstepcore*
  DEPENDS exp2cxx_exe_stage
  )

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/exp.stamp
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/exp2cxx ${CMAKE_CURRENT_SOURCE_DIR}/stepcode/data/ap203/ap203.exp
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/exp.stamp
  DEPENDS exp2cxx_exe_stage otool_run
  )
add_custom_target(exptest ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/exp.stamp)

std_build_rpath()

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8

