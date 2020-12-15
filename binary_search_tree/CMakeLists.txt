cmake_minimum_required(VERSION 2.8)
project(search_tree)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Wpedantic -Werror" ${CMAKE_C_FLAGS})

add_executable(search_tree src/search_tree.c src/search_tree_test.c)