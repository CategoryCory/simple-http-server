cmake_minimum_required(VERSION 3.22)
add_library(requests STATIC requests.c)
target_include_directories(requests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
