cmake_minimum_required(VERSION 3.22)
add_library(dictionary STATIC dictionary.c dict_utilities.c)
target_include_directories(dictionary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})