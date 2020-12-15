cmake_minimum_required(VERSION 3.5)
project(interview_practice)

# ------ gtest ---------
# http://www.kaizou.org/2014/11/gtest-cmake/
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

# ------ gtest ---------

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Wpedantic -Werror" ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -Wdelete-non-virtual-dtor -Wold-style-cast -Woverloaded-virtual")

add_executable(replace_spaces src/replace_spaces.cc)

add_executable(algorithm src/algorithm_test.cc)

add_executable(graph_cost src/graph_cost.cc src/graph_cost_test.cc)

add_library(libconvolution src/convolution.cc src/cache_memory.cc)

add_executable(convolution src/convolution_test.cc)
target_link_libraries(convolution libconvolution libgtest)

add_library(ratio_finder src/ratio_finder.cc) # src/ratio_finder_test.cc)
