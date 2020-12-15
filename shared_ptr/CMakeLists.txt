cmake_minimum_required(VERSION 2.8)
project(shared_ptr_practice)

execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_LESS 4.8)
 	message(FATAL_ERROR "GCC version must be >=4.8!")
endif()

message("C++ compiler version: " ${GCC_VERSION})

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror -Wdelete-non-virtual-dtor")

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

include_directories(include)

add_executable(test_shared_ptr
    test/test_shared_ptr.cpp
)
target_link_libraries(test_shared_ptr
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )

enable_testing()
add_test(test_shared_ptr test_shared_ptr)
