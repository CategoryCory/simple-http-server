cmake_minimum_required(VERSION 3.22)
add_library(server STATIC server.c)
target_include_directories(server PUBLIC 
                           ${CMAKE_CURRENT_SOURCE_DIR} 
                           ${CMAKE_CURRENT_SOURCE_DIR}/../dictionary
                           ${CMAKE_CURRENT_SOURCE_DIR}/../requests
                           ${CMAKE_CURRENT_SOURCE_DIR}/../file_utilities)
target_link_libraries(server PUBLIC dictionary requests file_utilities)