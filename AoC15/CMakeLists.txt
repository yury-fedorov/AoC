cmake_minimum_required(VERSION 3.16)
project(aoc15)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)
add_executable(aoc15 test.cpp 01/day.cpp 02/day.cpp 03/day.cpp 05/day.cpp)
target_link_libraries(aoc15 PRIVATE Catch2::Catch2)