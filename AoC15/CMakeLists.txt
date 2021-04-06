cmake_minimum_required(VERSION 3.16)
project(aoc15)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(Catch2 REQUIRED)
add_executable(aoc15 test.cpp 01/day.cpp 02/day.cpp 03/day.cpp 05/day.cpp 06/day.cpp 07/day.cpp 08/day.cpp 09/day.cpp 10/day.cpp
    11/day.cpp 12/day.cpp 13/day.cpp 14/day.cpp 15/day.cpp 16/day.cpp 17/day.cpp 18/day.cpp 19/day.cpp 20/day.cpp)


target_link_libraries(aoc15 PRIVATE Catch2::Catch2)
