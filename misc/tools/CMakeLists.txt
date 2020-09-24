#---------------------------------------------------------------------------------
# Local Copies of Tools
#---------------------------------------------------------------------------------

# Because we sometimes need to override system versions of tools, it
# is essential that the variables containing the executable to be run
# for each of the following tools use full paths - if just the name of
# the target is used, there is a chance that name is the same as the
# executable name itself.  In that case, if the system executable is
# in the path, it may get called instead of resolving to the compiled
# exectuable associated with the CMake target.  (Information from the
# CMake devs indicates this is not expected behavior, but at some
# point during the development efforts a problem apparently related to
# this came up.)  On single configuration systems we can reference
# specify the full path to the binary directory at CMake configure
# time, but this isn't possible for multi-configuration systems like
# Visual Studio since that path changes at run-time.  For those
# situations, CMake provides a variable CMAKE_CFG_INTDIR that will
# insert the correct logic for config-dependent paths.

# Clear all pre-defined CMake flags
CLEAR_BUILD_FLAGS()

# Restore CMake defaults, in case they are needed
RESTORE_CACHED_BUILD_FLAGS(_CMAKE_DEFAULT)

# For misc/tools, be liberal about standards
if(NOT MSVC)
  set(CMAKE_C_EXTENSIONS ON)
  set(CMAKE_C_STANDARD 99)
  set(CMAKE_C_STANDARD_REQUIRED OFF)
endif(NOT MSVC)

# Quiet all warnings in this directory
DISABLE_WARNINGS()

# Most third party items have a list calling out files for distcheck -
# these are stored in files in the dlists directory.  Ignore that
# directory for distcheck
file(GLOB dlists "*.dist")
foreach(ITEM ${dlists})
  get_filename_component(dlist ${ITEM} NAME)
  CMAKEFILES(${dlist})
endforeach(ITEM ${dlists})

# For those wanting to use a system version of the LEMON parser
# generator from sqlite, remember that the presence of /usr/bin/lemon
# is not enough.  LEMON needs a template file, lempar.c, and by
# default it needs it in the same directory as /usr/bin/lemon.  The
# typical approach to avoiding that requirement is to patch lemon,
# like this Gentoo ebuild:
#
# http://gentoo-overlays.zugaina.org/gentoo-zh/portage/dev-util/lemon/
#
# LEMON packages for other major Linux/BSD distros will do the same.
# BRL-CAD's FindLEMON.cmake macros will look for the template file in
# the executable directory first, and if not there will check in
# /usr/share/lemon (the location used by several distributions.)  If
# your distribution has a working lemon with the lempar.c template
# file in a custom location, specify the full path to the template
# with the variable LEMON_TEMPLATE - something like:
#
# cmake .. -DLEMON_TEMPLATE=/etc/lemon/lempar.c
#
# This is not to tell LEMON what template to use - that information is
# usually hardcoded in LEMON itself - but to let FindLEMON.cmake know
# there is a working LEMON installation.
set(LEMON_SKIP_INSTALL 1)
set(lemon_DESCRIPTION "
Option for enabling and disabling compilation of the lemon parser
provided with BRL-CAD's source distribution.  Default is AUTO,
responsive to the toplevel BRLCAD_BUNDLED_LIBS option and testing
first for a system version if BRLCAD_BUNDLED_LIBS is also AUTO.
")
THIRD_PARTY_EXECUTABLE(lemon LEMON lemon REQUIRED "BRLCAD_LEVEL2" ALIASES "ENABLE_LEMON" DESCRIPTION lemon_DESCRIPTION)
if("${BRLCAD_BUNDLED_LIBS}" STREQUAL "BUNDLED" OR "${BRLCAD_LEMON}" MATCHES "BUNDLED")
  set(LEMON_TEMPLATE "${BRLCAD_SOURCE_DIR}/misc/tools/lemon/lempar.c" CACHE STRING "Lemon template file" FORCE)
  set(LEMON_EXECUTABLE_TARGET "lemon" CACHE STRING "lemon target" FORCE)
endif("${BRLCAD_BUNDLED_LIBS}" STREQUAL "BUNDLED" OR "${BRLCAD_LEMON}" MATCHES "BUNDLED")
DISTCLEAN(${CMAKE_CURRENT_BINARY_DIR}/lemon/Makefile)
# Make sure we load FindLEMON.cmake to be able to define LEMON targets.
include(${BRLCAD_CMAKE_DIR}/FindLEMON.cmake)
SetTargetFolder(lemon "Compilation Utilities")
SetTargetFolder(lempar_cp "Compilation Utilities")

# Locac Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
