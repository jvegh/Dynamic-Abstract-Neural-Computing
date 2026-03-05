<<<<<<< HEAD
<<<<<<< HEAD
#https://medium.com/@techAsthetic/getting-started-with-systemc-setup-code-and-run-baad1943ec4b
#cmake_minimum_required(VERSION 3.0)


cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0007 NEW)
cmake_policy(SET CMP0071 NEW)
cmake_policy(SET CMP0084 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake $ENV{SYSTEMC_HOME})

# Choose whether to make documention as well
  option (BUILD_DOCS "Prepare also HTML/CHM documentation" ON)
# Choose whether you need debugging support for development or making a release
  option (DEBUG_MODE "Include debug support for the package"  ON)
# Choose whether you need to build GTest executables
  option(BUILD_GTESTS "Build Gtest tests" ON)
  string(TIMESTAMP PROJECT_TIMESTAMP "%Y%m%d")
  set(CPACK_PACKAGE_NAME "NeuronScQt")
  set(RIPES_GIT_VERSION "0.0.1")
    project(NeuronScQt VERSION ${RIPES_GIT_VERSION} LANGUAGES C CXX)
include(Colorize)
#include(Version)
#    message(HIGHLIGHTED "Configuring ${PROJECT_NAME} V${PROJECT_VERSION}@${PROJECT_TIMESTAMP} debug version")
include(BuildType)

find_Package(SystemC)
message(STATUS  "SYSTEMC_HOME path " $ENV{SYSTEMC_HOME})
message(STATUS  "SystemC compiler standard " ${SystemC_CXX_STANDARD})

set (SystemC_CXX_STANDARD "17")
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})
message("sc home =" ${SYSTEMC_HOME})

project(NeuronScQt
        LANGUAGES CXX
        DESCRIPTION "A SystemC-based time-aware neuron simulator with Qt GUI"
        HOMEPAGE_URL "https://github.com/jvegh/ScQtNeuron"
)

if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    include(GNUInstallDirs) #Must exist after declaring a project
endif()

# Error flags on everything but MSVC
if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wextra -Wall \
        -Wno-dev \
        -Werror=switch -Werror=return-type \
        -Werror=unreachable-code")
endif()

# Add the directory containing FindSystemCLanguage.cmake to the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SystemC_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD SystemC_CXX_STANDARD)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the SystemC include and library directories
set(SYSTEMC_INCLUDE_DIR $ENV{SYSTEMC_HOME}/include)
set(SYSTEMC_LIBRARY_DIR $ENV{SYSTEMC_HOME}/lib)


# Add the executable
add_executable(first_systemc main.cpp)

# Link against SystemC
target_include_directories(first_systemc PRIVATE ${SYSTEMC_INCLUDE_DIR})
target_link_libraries(first_systemc ${SYSTEMC_LIBRARY_DIR}/libsystemc.so)


######################################################################
## GUI setup
######################################################################


set(CMAKE_PREFIX_PATH "~/Qt/6.10.2/gcc_64" "~/systemc-3.0.2")
# Find required Qt packages
find_package(Qt6 COMPONENTS Core Widgets Svg Charts PrintSupport REQUIRED)

# Finding Qt includes
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Core_INCLUDE_DIRS})
include_directories(${Qt6Charts_INCLUDE_DIRS})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


######################################################################
## Resources setup
######################################################################
qt6_add_resources(ICONS_SRC ${CMAKE_SOURCE_DIR}/resources/icons/icons.qrc)
##??qt6_add_resources(EXAMPLES_SRC ${CMAKE_SOURCE_DIR}/examples/examples.qrc)
##??qt6_add_resources(LAYOUTS_SRC ${CMAKE_SOURCE_DIR}/src/processors/layouts.qrc)
qt6_add_resources(FONTS_SRC ${CMAKE_SOURCE_DIR}/resources/fonts/fonts.qrc)

######################################################################
## Library setup
######################################################################

# Header-only libraries used in the project must be included from top CMakeLists.txt file
include_directories(neuron)
include_directories(neuron/neurons)
include_directories(src/version)

# Platform specific pre-target setup(s)
if(${APPLE})
    set(SYSTEM_FLAGS MACOSX_BUNDLE)
elseif(${WIN32})
    set(SYSTEM_FLAGS WIN32)
endif()

# Fix the name of the libraries.
set(RIPES_LIB ripes_lib)
set(NEURON_LIB neuron_lib)
set(VERSION_LIB version_lib)
add_subdirectory(src)
add_subdirectory(neuron)
add_subdirectory(3rdParty/qcustomplot)
add_subdirectory(3rdParty/fancytabbar)

add_subdirectory(demos)

# ---[ Install GTest
#if(BUILD_TESTS)
    find_package(GTest REQUIRED)
    add_subdirectory(gtest)
    message(HIGHLIGHTED "Testing with GTest installed")
#endif()

if(BUILD_DOCS)
  file(COPY ${CMAKE_SOURCE_DIR}/doc/dynsections.js DESTINATION ${CMAKE_BINARY_DIR}/doc)
  find_package(Doxygen)
##  add_subdirectory(doc) # The doxygen documentation
endif(BUILD_DOCS)

set(APP_NAME NeuronScQt)
qt_add_executable(${APP_NAME} ${SYSTEM_FLAGS} ${ICONS_SRC} #${EXAMPLES_SRC} ${LAYOUTS_SRC}
${FONTS_SRC} main.cpp)

# Link Qt libraries
#target_link_libraries(${APP_NAME} PUBLIC Qt6::Core Qt6::Widgets)
# Link Ripes library
target_link_libraries(${APP_NAME} PUBLIC ${RIPES_LIB}
                                  ${FANCYTABBAR_LIB}
                                  ${NEURON_LIB}
                                  version_lib
                                  SystemC::systemc
                                  Qt6::Core Qt6::Widgets
                                  QCustomPlot  Qt6::PrintSupport
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    # https://doc.qt.io/qt-6/wasm.html#asyncify
    target_link_options(${RIPES_LIB} PUBLIC -sASYNCIFY -Os)
    target_link_options(${APP_NAME} PUBLIC -sASYNCIFY -Os)
endif()

link_directories(
  ${NEURON_LIB}
  ../3rdParty/qcustomplot/
    ${SystemC_LIBRARY_DIRS}
    /opt/systemc/lib
)

include(Summary)

if(UNIX AND NOT APPLE) #Define the LINUX variable before testing it
    set(LINUX TRUE)
endif()

if(${LINUX})
    install(TARGETS ${APP_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
elseif(${APPLE})
    install(TARGETS ${APP_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE  DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/appdir/usr/
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)
=======
=======
>>>>>>> doc
cmake_minimum_required(VERSION 3.22.1)
project(QT6HW)


set(SYSTEMC_HOME /opt/systemc)
set(SYSTEMC_INCLUDE_DIR $ENV{SYSTEMC_HOME}/include)
set(SYSTEMC_LIBRARY_DIR $ENV{SYSTEMC_HOME}/lib)
#set(SystemCLanguage_DIR "~/systemc-3.0.2/build")
set(CMAKE_PREFIX_PATH "~/Qt/6.10.2/gcc_64")
set(CMAKE_PREFIX_PATH "~/systemc-3.02/build")
#set(Qt6_DIR=$HOME/Qt/6.10.2/gcc_64)
find_package(Qt6 COMPONENTS Core Widgets)
set(CMAKE_MODULE_PATH  "~/systemc-3.0.2/build")
set(CMAKE_CXX_STANDARD 17)
find_package(SystemCLanguage)

# We need to add .h files (at least those that define a custom Qt class), see https://stackoverflow.com/a/14011060
qt_add_executable(${PROJECT_NAME} src/main.cpp src/MyWindow.cpp include/MyWindow.h)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
target_include_directories(${PROJECT_NAME} PUBLIC include ${SYSTEMC_INCLUDE_DIR})


target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Widgets ${SYSTEMC_LIBRARY_DIR}/libsystemc.so)
<<<<<<< HEAD

>>>>>>> code
=======
>>>>>>> doc
