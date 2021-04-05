cmake_minimum_required(VERSION 3.16)
project(aoc20)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)
add_executable(aoc20 test.cpp 01/day.cpp 02/day.cpp 03/day.cpp)
target_link_libraries(aoc20 PRIVATE Catch2::Catch2)
