cmake_minimum_required(VERSION 3.17) # 3.20 for CXX_STANDARD 23, 3.18 for Ubuntu, 3.17 for Windows
project(aoc19)

set(CMAKE_CXX_STANDARD 20) # due to issues with range-v3
#  set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-11") # does not work on Mac

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,memory,undefined,dataflow")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,memory,undefined,dataflow")

# find_package(Catch2 REQUIRED) # for testing
# find_package(absl REQUIRED) # Status, StatusOr
# find_package(fmt REQUIRED) # as CLang is not supporting yet std::format
# find_package(Microsoft.GSL REQUIRED)

include(FetchContent)

# In this example we are picking a specific tag.
# You can also pick a specific commit, if you need to.
FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v3.1.0" )

FetchContent_Declare(range-v3
        GIT_REPOSITORY "https://github.com/ericniebler/range-v3"
        GIT_TAG "0.11.0" )

FetchContent_Declare(fmt
        GIT_REPOSITORY "https://github.com/fmtlib/fmt"
        GIT_TAG "8.0.1" )

FetchContent_Declare(Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2"
        GIT_TAG "v2.13.7" )

FetchContent_MakeAvailable(GSL)
FetchContent_MakeAvailable(range-v3)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(Catch2)

file( GLOB SOURCES day*.cpp common.cpp intcode_computer.cpp )
set(CMAKE_CXX_CPPCHECK "cppcheck")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
add_executable(aoc19 test.cpp ${SOURCES}  )
# Microsoft.GSL::GSL
# absl::strings
# absl::status 
# fmt::fmt
target_link_libraries(aoc19 PRIVATE Catch2::Catch2 GSL fmt range-v3 )
