set(CMAKE_CXX_STANDARD 14)

# 面试中的一些算法
add_executable(cxx_sort sort/test_sort.cc sort/test_2array.h)

# 加入测试boost_context协程上下文底程
add_executable(cxx_context boost/context.cc boost/uthread.h boost/uthread.inl)
target_link_libraries(cxx_context boost_context)

add_executable(cxx_demo cxx11/main.cc )
target_link_libraries(cxx_demo absl::base absl::strings)

add_executable(jvm_demo jvm/jvm_simple.cpp jvm/oop.hpp )
target_link_libraries(jvm_demo)

# gtest 例子.
if (BUILD_TESTING)
    add_executable(cxx_demo_test cxx11/size_test.cpp)
    target_link_libraries(cxx_demo_test gtest_main)
    add_test(cxx_demo_test cxx_demo_test)
endif ()