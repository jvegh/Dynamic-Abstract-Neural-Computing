# This is the CMakeLists file for generating a conceptual Action Potential
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "                    scQtSimulator ")


include_directories(
    include
    ../../neuron/include
    ../../neuron/neurons/include
    ../../QtGUI/include
    ../../3rdParty/qcustomplot/include/
    ../../3rdParty/fancytabbar
#    /opt/systemc/include
    ${SystemC_INCLUDE_DIR}
    )


link_directories(
  ${NEURON_LIB}
    ${SystemC_LIBRARY_DIRS}
    /opt/systemc/lib
    )

set(DEMO_SOURCES
#   AP_Conceptual.cpp
scQtNeuron_MainWindow.cpp
scQtNeuron_Main.cpp
##scQMainGUI.cpp
##scQSimulator.cpp
scQAbout.cpp
##mainwindow.cpp
##include/mainwindow.h
scQtSimulator.cpp
#scQtThread.cpp
#scQtModule.cpp
)


ADD_EXECUTABLE(
    scQtSimulator
    ${DEMO_SOURCES}
)

       target_compile_features(scQtSimulator PRIVATE cxx_std_17)

target_link_libraries(
    scQtSimulator
      ${NEURON_LIB}
      ${QTGUI_LIB}
      ${SystemC_LIBRARIES}
#       pthread
)

install(TARGETS scQtSimulator
   RUNTIME
   DESTINATION bin
   COMPONENT demos)


INSTALL(FILES ${DEMO_SOURCES} CMakeLists.txt main.cpp
	DESTINATION demo
	COMPONENT srcs)

INSTALL(FILES ${DEMO_INCLUDES}
	DESTINATION demo/include
	COMPONENT srcs)


