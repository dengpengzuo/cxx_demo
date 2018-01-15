cmake_minimum_required(VERSION 2.8)

project(cxx_demo)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
## gcc 编译参数
set(CMAKE_CXX_COMPILE_FEATURES "-Wall ${CMAKE_CXX_COMPILE_FEATURES} ")
## clang 链接参数
## set(CMAKE_CXX_FLAGS "-std=c++14 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")

# cxx特性学习
add_subdirectory(deps/googletest)
add_subdirectory(deps/cctz)
add_subdirectory(deps/abseil-cpp)
# cxx_demo project
add_subdirectory(src)