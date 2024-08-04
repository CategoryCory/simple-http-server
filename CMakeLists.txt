cmake_minimum_required(VERSION 3.22)

# Set project name and specify language
project(simple_http VERSION 0.1 LANGUAGES C)

# Set C Standard and flags
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -O2")

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-2.0)

# Include required directories
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GIO_INCLUDE_DIRS})

# Add project subdirectories
add_subdirectory(src/server)
add_subdirectory(src/http)
add_subdirectory(src/utils)

# Add executable file
add_executable(main src/main.c)

# Add include directories
target_include_directories(main PUBLIC
"${PROJECT_SOURCE_DIR}/src/server"
"${PROJECT_SOURCE_DIR}/src/http"
"${PROJECT_SOURCE_DIR}/src/utils"
)

# Add GLib compiler flags
target_compile_options(main PRIVATE ${GLIB_CFLAGS_OTHER} ${GIO_CFLAGS_OTHER})

# Add target libraries
target_link_libraries(main PUBLIC server request_parser ${GLIB_LIBRARIES} ${GIO_LIBRARIES})