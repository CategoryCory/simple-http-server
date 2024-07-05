cmake_minimum_required(VERSION 3.22)
project(simple_http VERSION 0.1)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
add_subdirectory(dictionary)
add_executable(server main.c)
target_link_libraries(server PUBLIC dictionary)
target_include_directories(server PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/dictionary"
                           )