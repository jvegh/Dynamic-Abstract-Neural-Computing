# This is the CMakeLists file for generating a conceptual Action Potential
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "                    Conceptual Action Potential ")
#  find_package(SystemC REQUIRED)


include_directories(
    include
    ../../neuron/include
    ../../neuron/neurons/include
    ${SystemC_INCLUDE_DIRS}
#    ${CMAKE_INSTALL_PREFIX}/include
    /opt/systemc/include
    include
    )


link_directories(
  ${NEURON_LIB}
  ${QTGUI_LIB}
    ${SystemC_LIBRARY_DIR}
)

set(DEMO_SOURCES
   AP_Conceptual.cpp
)


ADD_EXECUTABLE(
    ${PROJECT_NAME}_AP_Conceptual
    ${PROJECT_NAME}_AP_Conceptual.cpp
#    ${DEMO_SOURCES}
)

target_compile_features(${PROJECT_NAME}_AP_Conceptual PRIVATE cxx_std_17)

target_link_libraries(
    ${PROJECT_NAME}_AP_Conceptual
      ${NEURON_LIB}
      ${SystemC_LIBRARIES}
      ${NEURON_LIB}
      ${SystemC_LIBRARIES}
      /opt/systemc/lib/libsystemc.so
#       pthread
)


link_directories(
    ${SystemC_LIBRARY_DIRS}
#    ${SCV_LIBRARY_DIRS}
)

install(TARGETS ${PROJECT_NAME}_AP_Conceptual
   RUNTIME
   DESTINATION bin
   COMPONENT demos
)


INSTALL(FILES ${DEMO_SOURCES} CMakeLists.txt ${PROJECT_NAME}_AP_Conceptual.cpp
	DESTINATION demo
	COMPONENT srcs)

INSTALL(FILES ${DEMO_INCLUDES}
	DESTINATION demo/include
	COMPONENT srcs
)


