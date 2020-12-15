cmake_minimum_required(VERSION 2.8)
project(linked_list)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Wpedantic" ${CMAKE_C_FLAGS})

add_executable(linked_list src/linked_list.c src/test_linked_list.c)