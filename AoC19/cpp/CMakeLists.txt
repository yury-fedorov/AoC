cmake_minimum_required(VERSION 3.20)
project(aoc19)

set(CMAKE_CXX_STANDARD 23)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(Catch2 REQUIRED)
file( GLOB SOURCES day*.cpp )
set(CMAKE_CXX_CPPCHECK "cppcheck")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
add_executable(aoc19 test.cpp ${SOURCES} )

target_link_libraries(aoc19 PRIVATE Catch2::Catch2)
