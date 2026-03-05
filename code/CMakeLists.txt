cmake_minimum_required(VERSION 3.22.1)
project(QT6HW)


set(SYSTEMC_HOME /opt/systemc)
set(SYSTEMC_INCLUDE_DIR $ENV{SYSTEMC_HOME}/include)
set(SYSTEMC_LIBRARY_DIR $ENV{SYSTEMC_HOME}/lib)
set(CMAKE_PREFIX_PATH "~/systemc-3.0.2/build")
find_package(SystemCLanguage)
#set(Qt6_DIR=$HOME/Qt/6.10.2/gcc_64)
set(CMAKE_PREFIX_PATH "~/Qt/6.10.2/gcc_64")
find_package(Qt6 COMPONENTS Core Widgets)
set(CMAKE_CXX_STANDARD 17)

# We need to add .h files (at least those that define a custom Qt class), see https://stackoverflow.com/a/14011060
qt_add_executable(${PROJECT_NAME} src/main.cpp src/MyWindow.cpp include/MyWindow.h)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
target_include_directories(${PROJECT_NAME} PUBLIC include ${SYSTEMC_INCLUDE_DIR})


target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Widgets ${SYSTEMC_LIBRARY_DIR}/libsystemc.so)

