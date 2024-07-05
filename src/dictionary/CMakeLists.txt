cmake_minimum_required(VERSION 3.22)
add_library(dictionary dictionary.c)
target_include_directories(dictionary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_compile_options()