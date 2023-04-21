cmake_minimum_required(VERSION 3.10)
project(interview_practice)

# Built GTest with -DBUILD_SHARED_LIBS=ON
# Must set LD_LIBRARY_PATH to include /usr/local/lib
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -Wdelete-non-virtual-dtor -Wold-style-cast -Woverloaded-virtual")

IF(APPLE)
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    LINK_DIRECTORIES(/usr/local/lib)
    INCLUDE_DIRECTORIES(/usr/local/include)
ENDIF()

add_executable(replace_spaces src/replace_spaces.cc)

add_executable(algorithm_test src/algorithm_test.cc)
target_link_libraries(algorithm_test pthread gtest gtest_main)
gtest_add_tests(TARGET      algorithm_test
  SOURCES src/algorithm_test.cc)

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

add_library(
  prefix_dict src/prefix_dict.cc)
add_executable(prefix_dict_test src/prefix_dict_test.cc)
target_link_libraries(prefix_dict_test prefix_dict pthread gtest gtest_main)
gtest_add_tests(TARGET      prefix_dict_test
  SOURCES src/prefix_dict_test.cc)

add_executable(list_splitter_test src/list_splitter_test.cc)
target_link_libraries(list_splitter_test pthread gtest gtest_main)
gtest_add_tests(TARGET      list_splitter_test
  SOURCES src/list_splitter_test.cc)
