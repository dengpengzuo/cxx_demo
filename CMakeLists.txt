cmake_minimum_required(VERSION 2.8)

project(cxx_demo)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(SRC_FILES src/main.cc)

add_executable(cxx_demo ${SRC_FILES})