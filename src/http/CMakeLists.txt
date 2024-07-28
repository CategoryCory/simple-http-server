cmake_minimum_required(VERSION 3.22)

# Add library
add_library(request_parser SHARED request_parser.c)

# Add include directories and target libraries
target_link_libraries(request_parser PUBLIC string_utilities ${GLIB_LIBRARIES})
target_include_directories(request_parser PUBLIC 
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/../string_utilities
                           ${GLIB_INCLUDE_DIRS})

# Add GLib compiler flags
target_compile_options(request_parser PRIVATE ${GLIB_CFLAGS_OTHER})