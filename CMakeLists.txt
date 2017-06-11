cmake_minimum_required(VERSION 2.8)

project(cxx_demo)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_COMPILE_FEATURES "${CMAKE_CXX_COMPILE_FEATURES} -Wall -std=c++11")

add_executable(cxx_demo src/main.cc)

# 加入测试boost_context协程上下文底程
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_context")
add_executable(context src/context.cc)