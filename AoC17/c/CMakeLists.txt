cmake_minimum_required(VERSION 3.21)
project(aoc17)

set(CMAKE_C_STANDARD 23)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

file(GLOB SOURCES day23.c)
add_executable(aoc17 ${SOURCES})

target_link_libraries(aoc17 PRIVATE)
