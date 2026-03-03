# This is the CMakeLists file generating Qt6-based QCustomPlot library stuff
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "                    QCustomPlot library")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
    set(DESIRED_QT_VERSION 6)
set(CMAKE_PREFIX_PATH "/home/jvegh/Qt/6.4.1/gcc_64")
find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets Concurrent PrintSupport)# Widgets finds its own dependencies.
qt_standard_project_setup()

  # The Qt6Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
include_directories(
      ${Qt6Core_INCLUDE_DIRS}
      ${Qt6Gui_INCLUDE_DIRS}
      ${Qt6Widgets_INCLUDE_DIRS}
      ${Qt6PrintSupport_INCLUDE_DIRS}
      include
      )

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 6.
add_definitions(${Qt6Widgets_DEFINITIONS} ${Qt6Printer_DEFINITIONS})

QT6_WRAP_CPP(qcustomplot_moc include/qcustomplot.h)

  # Executables fail to build with Qt 6 in the default configuration
  # without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt6Widgets_EXECUTABLE_COMPILE_FLAGS}  -std=c++11 -Wall")

ADD_LIBRARY(QCustomPlot
    qcustomplot.cpp
    ${qcustomplot_moc} # The MOC headers, generated
)

set_target_properties(QCustomPlot
                      PROPERTIES OUTPUT_NAME QCustomPlot
                      )

INSTALL(FILES  qcustomplot.cpp
	DESTINATION 3rdParty/QCustomPlot
	COMPONENT srcs)

INSTALL(FILES  include/qcustomplot.h
	DESTINATION 3rdParty/QCustomPlot/include
	COMPONENT srcs)


