cmake_minimum_required(VERSION 3.18)

project(rotating_cube)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS FALSE)
# See `man 7 feature_test_macros`. This option is needed to enable nanosleep and struct timespec.
add_definitions(-D_POSIX_C_SOURCE=199309L)

add_subdirectory(src)

