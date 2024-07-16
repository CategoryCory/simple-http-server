cmake_minimum_required(VERSION 3.22)

# Add library
add_library(requests STATIC requests.c)

# Add include directories and target libraries
target_link_libraries(requests PUBLIC string_utilities ${GLIB_LIBRARIES})
target_include_directories(requests PUBLIC 
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/../string_utilities
                           ${GLIB_INCLUDE_DIRS})

# Add GLib compiler flags
target_compile_options(requests PRIVATE ${GLIB_CFLAGS_OTHER})