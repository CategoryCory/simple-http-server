cmake_minimum_required(VERSION 3.22)
add_library(string_utilities STATIC string_utilities.c)
target_include_directories(string_utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})