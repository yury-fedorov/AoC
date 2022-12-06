cmake_minimum_required(VERSION 3.17) # 3.20 for CXX_STANDARD 23, 3.18 for Ubuntu, 3.17 for Windows
project(aoc22)

set(CMAKE_CXX_STANDARD 20) # 3.20 for CXX_STANDARD 23, latest Ubuntu 22.10 3.24
set(ABSL_PROPAGATE_CXX_STD, ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wno-deprecated -fno-exceptions -fno-rtti) # -Wall -Wextra -Werror -pedantic
endif()

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,memory,undefined,dataflow")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,memory,undefined,dataflow")

include(FetchContent)

FetchContent_Declare(absl
    GIT_REPOSITORY "https://github.com/abseil/abseil-cpp.git"
    GIT_TAG "20220623.1" )

FetchContent_Declare(gtest
    # URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG "release-1.12.1" )

FetchContent_MakeAvailable(absl)
FetchContent_MakeAvailable(gtest)

file( GLOB SOURCES day*.cpp )
# set(CMAKE_CXX_CPPCHECK "cppcheck")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
add_executable(aoc22 test.cpp ${SOURCES}  )

# absl::status 
target_link_libraries(aoc22 PRIVATE absl::flat_hash_set absl::strings gtest)
