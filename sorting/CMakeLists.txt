cmake_minimum_required(VERSION 3.1)
project(sorting_practice)

set(CMAKE_BUILD_TYPE RelWidthDebInfo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -Wdelete-non-virtual-dtor")

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
