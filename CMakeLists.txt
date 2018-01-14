cmake_minimum_required(VERSION 2.8)

project(cxx_demo)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
## clang 编译参数
## set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_COMPILE_FEATURES "${CMAKE_CXX_COMPILE_FEATURES} -Wall")

include_directories(src)

# 常用排序算法
add_executable(cxx_sort src/sort/test_sort.cc)
TARGET_LINK_LIBRARIES(cxx_sort profiler)

# 加入测试boost_context协程上下文底程
add_executable(cxx_context src/boost/context.cc src/boost/uthread.h src/boost/uthread.inl)
TARGET_LINK_LIBRARIES(cxx_context boost_context)

# cxx特性测试
add_subdirectory(deps/googletest)
#add_subdirectory(deps/cctz)
#add_subdirectory(deps/abseil-cpp)
add_executable(cxx_demo src/cxx11/main.cc)