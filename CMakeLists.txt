cmake_minimum_required(VERSION 3.0)
project(cxx_demo)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/lib)

#set(CMAKE_CXX_COMPILER "/usr/local/clang/bin/clang++")      #使用clang
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -stdlib=libc++")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(SOURCE_FILES src/main.cc)

add_executable(cxx_util ${SOURCE_FILES})
#target_link_libraries(cxx_util c++abi)  #clang必须加上引用c++abi,
set_target_properties(cxx_util PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")
