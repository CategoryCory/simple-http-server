cmake_minimum_required(VERSION 3.22)
project(simple_http VERSION 0.1)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -O2")
add_subdirectory(dictionary)
add_subdirectory(server)
add_executable(main main.c)
target_link_libraries(main PUBLIC server)
target_include_directories(main PUBLIC
                           "${PROJECT_SOURCE_DIR}/dictionary"
                           "${PROJECT_SOURCE_DIR}/server"
                           )