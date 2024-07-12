cmake_minimum_required(VERSION 3.22)
add_library(file_utilities STATIC file_utilities.c)
target_include_directories(file_utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})