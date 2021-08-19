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
file( GLOB SOURCES */*.cpp )
set(CMAKE_CXX_CPPCHECK "cppcheck")
# a lot of warnings 
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
add_executable(aoc15 test.cpp ${SOURCES} )

target_link_libraries(aoc15 PRIVATE Catch2::Catch2)
