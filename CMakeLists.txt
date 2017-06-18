cmake_minimum_required(VERSION 2.8)

project(cxx_demo)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_COMPILE_FEATURES "${CMAKE_CXX_COMPILE_FEATURES} -Wall -std=c++11")

include_directories(src)

# cxx特性测试
add_executable(cxx_demo src/cxx11/main.cc)

# 常用排序算法
add_executable(cxx_sort src/sort/test_sort.cc)

# 加入测试boost_context协程上下文底程
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_context")
add_executable(cxx_context src/boost/context.cc)