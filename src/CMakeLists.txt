add_executable(rotating_cube rotating_cube.c gfutils.c)
target_link_libraries(rotating_cube m)

add_compile_options(-Wall -Werror -pedantic)

install(TARGETS rotating_cube DESTINATION ${CMAKE_SOURCE_DIR}/dist)

if (APPLE)
    include (${CMAKE_SOURCE_DIR}/cmake/macos.cmake)
endif ()
