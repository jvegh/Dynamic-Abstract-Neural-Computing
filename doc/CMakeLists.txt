
cmake_minimum_required(VERSION 3.22.1)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0007 NEW)
cmake_policy(SET CMP0071 NEW)
cmake_policy(SET CMP0084 NEW)
cmake_policy(SET CMP0153 OLD)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${SYSTEMC_HOME})

# ---[ Set up CMake options for the simulator
# Choose if clocked physical bus is to be used; slows down simulation; not yet in use
  option (USE_CLOCKED_BUS "Simulate clocked physical bus" OFF)
# Choose whether you need debugging support for development or making a release
  option (DEBUG_MODE "Include debug support for the package"  ON)
# Choose whether the stand-alone unit testing is to be built
  option (BUILD_GTESTS "Include unit tests for the package"  ON)
# Choose whether to make documention as well
  option (BUILD_DOCS "Prepare also HTML/CHM documentation" ON)
# Choose whether you need internal information to docs
  option (DOXY_INTERNAL "Include internal info in the docs"  YES)
# ---[ End up CMake options for the user

#include(../code/cmake/Colorize)

    set(MyVersion "0.0.6")
    set(MyDate "2026.04.13")

  project(NeuronScQt VERSION ${MyVersion}
        LANGUAGES C CXX
        DESCRIPTION Description
        HOMEPAGE_URL "https://jvegh.github.io/Dynamic-Abstract-Neural-Computing"
)

  string(TIMESTAMP PROJECT_TIMESTAMP "%Y%m%d")
  set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

  string(TIMESTAMP PROJECT_TIMESTAMP "%Y%m%d")
  set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
# Print build information
#include(../code/cmake/BuildType)

######################################################################
##  Doxygen setup
######################################################################

if(BUILD_DOCS)
  find_package(Doxygen)
  add_subdirectory(doxy) # The doxygen documentation
  set(doxyfile_in "doxy/Doxyfile.in")
endif(BUILD_DOCS)

