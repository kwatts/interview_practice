cmake_minimum_required(VERSION 3.5)
project(path_planning)

# Built GTest with -DBUILD_SHARED_LIBS=ON
# Must set LD_LIBRARY_PATH to include /usr/local/lib
find_package(GTest REQUIRED)
include(GoogleTest)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -Wdelete-non-virtual-dtor")

include_directories(include)

add_library(
  djikstra_planner src/djikstra_planner.cc)
add_executable(djikstra_planner_test test/djikstra_planner_test.cc)
target_link_libraries(djikstra_planner_test djikstra_planner pthread gtest gtest_main)
gtest_add_tests(TARGET      djikstra_planner_test)

