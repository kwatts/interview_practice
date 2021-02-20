cmake_minimum_required(VERSION 2.8)
project(linked_list)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-Wall -Wpedantic -Werror" ${CMAKE_C_FLAGS})


add_executable(linked_list src/linked_list.c src/test_linked_list.c)
