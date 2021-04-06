cmake_minimum_required(VERSION 3.16)
project(aoc20)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(Catch2 REQUIRED)
#file( GLOB SOURCES */*.cpp )
#add_executable(aoc20 test.cpp ${SOURCES} )
add_executable(aoc20 test.cpp 01/day.cpp 02/day.cpp 03/day.cpp 04/day.cpp 05/day.cpp 06/day.cpp 07/day.cpp 08/day.cpp)
target_link_libraries(aoc20 PRIVATE Catch2::Catch2)
