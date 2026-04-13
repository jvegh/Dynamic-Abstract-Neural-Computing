#https://medium.com/@techAsthetic/getting-started-with-systemc-setup-code-and-run-baad1943ec4b

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

include(Colorize)

    set(MyVersion "0.0.6")
    set(MyDate "2026.04.13")

  project(NeuronScQt VERSION ${MyVersion}
        LANGUAGES C CXX
        DESCRIPTION Description
        HOMEPAGE_URL "https://jvegh.github.io/Dynamic-Abstract-Neural-Computing"
)

  string(TIMESTAMP PROJECT_TIMESTAMP "%Y%m%d")
  set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
# Print build information
include(BuildType)


######################################################################
## Install and set up  SystemC
######################################################################
set(SYSTEMC_HOME "/opt/systemc")
set(SYSTEMC_INCLUDE_DIR ${SYSTEMC_HOME}/include)
set(SYSTEMC_LIBRARY_DIR ${SYSTEMC_HOME}/lib)
set(SystemC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIR})

set(CMAKE_PREFIX_PATH "~/systemc-3.0.2/build")
find_package(SystemCLanguage)


######################################################################
## Install and set up GUI
######################################################################
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.10.2/gcc_64")
find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets Concurrent PrintSupport)
set(CMAKE_CXX_STANDARD 17)

set(SystemC_CXX_STANDARD ${CMAKE_CXX_STANDARD})
set(SystemC_CXX_STANDARD_REQUIRED )
# Set Qt includes
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Core_INCLUDE_DIRS})
include_directories(${Qt6Charts_INCLUDE_DIRS})

include_directories(QtGUI)
include_directories(neuron/include)
include_directories(neuron/neurons/include)

######################################################################
## Resources setup
######################################################################
qt6_add_resources(ICONS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/icons.qrc)
#qt6_add_resources(EXAMPLES_SRC examples/examples.qrc)
#qt6_add_resources(LAYOUTS_SRC ${CMAKE_SOURCE_DIR}/src/processors/layouts.qrc)
qt6_add_resources(FONTS_SRC resources/fonts/fonts.qrc)


message(HIGHLIGHTED "Configuring 'NeuronScQt' libraries")

add_subdirectory(3rdParty/qcustomplot)
add_subdirectory(3rdParty/fancytabbar)

set(NEURON_LIB neuron_lib)
set(QTGUI_LIB qtgui_lib)
add_subdirectory(neuron)
add_subdirectory(QtGUI)
add_subdirectory(src)
add_subdirectory(demos)

######################################################################
##  GTest setup
######################################################################

if(BUILD_GTESTS)
    find_package(GTest REQUIRED)
    add_subdirectory(gtest)
    message(HIGHLIGHTED "Testing with GTest installed")
endif()


######################################################################
##  Doxygen setup
######################################################################

if(BUILD_DOCS)
  find_package(Doxygen)
#  add_subdirectory(/home/jvegh/REPO/Dynamic-Abstract-Neural-Computing/doc) # The doxygen documentation
  set(doxyfile_in "${CMAKE_SOURCE_DIR}/doc/Doxyfile.in")
endif(BUILD_DOCS)

# We need to add .h files (at least those that define a custom Qt class), see https://stackoverflow.com/a/14011060
qt_add_executable(${PROJECT_NAME}  ${SYSTEM_FLAGS} ${ICONS_SRC} ${FONTS_SRC} #${EXAMPLES_SRC} ${LAYOUTS_SRC}
  src/main.cpp src/MyWindow.cpp include/MyWindow.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
target_include_directories(${PROJECT_NAME} PUBLIC include ${SYSTEMC_INCLUDE_DIR})

target_link_directories(${PROJECT_NAME} PUBLIC
      3rdParty/qcustomplot
      neuron
      QtGUI
      src/version
)

target_link_libraries(${PROJECT_NAME} PUBLIC
                      ${QTGUI_LIB}
                      QCustomPlot
                      version_lib
                      ${NEURON_LIB}
                      ${SYSTEMC_LIBRARY_DIR}/libsystemc.so
                      Qt6::Core Qt6::Widgets Qt6::PrintSupport
    )

# ---[ Target 'package' builds everything
include(cmake/Packages.cmake)
