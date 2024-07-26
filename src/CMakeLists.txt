cmake_minimum_required(VERSION 3.22)

# Set project name and specify language
project(simple_http VERSION 0.1 LANGUAGES C)

# Set C Standard and flags
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -O2")

# Add project subdirectories
add_subdirectory(server)
add_subdirectory(request_parser)
add_subdirectory(file_utilities)
add_subdirectory(string_utilities)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-2.0)

# Include required directories
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GIO_INCLUDE_DIRS})

# Add executable file
add_executable(main main.c)

# Add target libraries and include directories
target_link_libraries(main PUBLIC server request_parser ${GLIB_LIBRARIES} ${GIO_LIBRARIES})
target_include_directories(main PUBLIC
                           "${PROJECT_SOURCE_DIR}/server"
                           "${PROJECT_SOURCE_DIR}/request_parser"
                           "${PROJECT_SOURCE_DIR}/file_utilities"
                           "${PROJECT_SOURCE_DIR}/string_utilities"
                           )

# Add GLib compiler flags
target_compile_options(main PRIVATE ${GLIB_CFLAGS_OTHER} ${GIO_CFLAGS_OTHER})