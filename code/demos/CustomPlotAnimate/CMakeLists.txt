# This is the CMakeLists file for generating a conceptual Action Potential
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "                    CustomPlot demo ")
  find_package(SystemC REQUIRED)


include_directories(
    include
    ../../neuron
    ../../neuron/neurons
    ../../3rdParty/qcustomplot/include
    ${SystemC_INCLUDE_DIRS}
    ${CMAKE_INSTALL_PREFIX}/include
    )


link_directories(
  ${NEURON_LIB}
  ../3rdParty/qcustomplot/
    ${SystemC_LIBRARY_DIRS}
)

set(DEMO_SOURCES
   main.cpp
   mainwindow.cpp
)


ADD_EXECUTABLE(
    ${PROJECT_NAME}_CustomPlot_Animate
    ${DEMO_SOURCES}
)

       target_compile_features(${PROJECT_NAME}_CustomPlot_Animate PRIVATE cxx_std_17)

target_link_libraries(
    ${PROJECT_NAME}_CustomPlot_Animate
#      ${NEURON_LIB}
      QCustomPlot
      Qt6::PrintSupport
      ${SystemC_LIBRARIES}
#       pthread
)

install(TARGETS ${PROJECT_NAME}_CustomPlot_Animate
   RUNTIME
   DESTINATION bin
   COMPONENT demos)


INSTALL(FILES ${DEMO_SOURCES} CMakeLists.txt ${PROJECT_NAME}_AP_Conceptual.cpp
	DESTINATION demo
	COMPONENT srcs)

INSTALL(FILES ${DEMO_INCLUDES}
	DESTINATION demo/include
	COMPONENT srcs)


