cmake_minimum_required(VERSION 3.5)
project(interview_practice)

# Built GTest with -DBUILD_SHARED_LIBS=ON
# Must set LD_LIBRARY_PATH to include /usr/local/lib
find_package(GTest REQUIRED)
include(GoogleTest)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Wpedantic -Werror" ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -Wdelete-non-virtual-dtor -Wold-style-cast -Woverloaded-virtual")

add_executable(replace_spaces src/replace_spaces.cc)

add_executable(algorithm src/algorithm_test.cc)

# Graph basics
add_library(simple_graph src/simple_graph.cc)

add_library(
  graph_cost src/graph_cost.cc)
add_executable(graph_cost_test src/graph_cost_test.cc)
target_link_libraries(graph_cost_test simple_graph graph_cost pthread gtest gtest_main)
gtest_add_tests(TARGET      graph_cost_test
  SOURCES src/graph_cost_test.cc)

add_library(libconvolution src/convolution.cc src/cache_memory.cc)
add_executable(convolution_test src/convolution_test.cc)
target_link_libraries(convolution_test libconvolution pthread gtest gtest_main)
gtest_add_tests(TARGET      convolution_test
  SOURCES src/convolution_test.cc)


add_library(libratio_finder src/ratio_finder.cc)
target_link_libraries(libratio_finder simple_graph)

add_executable(ratio_finder_test src/ratio_finder_test.cc)
target_link_libraries(ratio_finder_test libratio_finder pthread gtest gtest_main)
gtest_add_tests(TARGET ratio_finder_test
  SOURCES src/ratio_finder_test.cc)

