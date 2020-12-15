cmake_minimum_required(VERSION 2.8)
project(sorting_practice)

set(CMAKE_BUILD_TYPE RelWidthDebInfo)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -Wdelete-non-virtual-dtor")

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

include_directories(include)

add_executable(test_sorting
    test/test_sorting.cpp
)
target_link_libraries(test_sorting
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )

enable_testing()
add_test(test_sorting test_sorting)
