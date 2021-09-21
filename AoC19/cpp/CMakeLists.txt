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

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,memory,undefined,dataflow")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,memory,undefined,dataflow")

find_package(Catch2 REQUIRED) # for testing
find_package(absl REQUIRED) # Status, StatusOr
find_package(fmt REQUIRED) # as CLang is not supporting yet std::format
# find_package(Microsoft.GSL REQUIRED)


include(FetchContent)

# In this example we are picking a specific tag.
# You can also pick a specific commit, if you need to.
FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v3.1.0"
        )

FetchContent_MakeAvailable(GSL)

file( GLOB SOURCES day*.cpp )
set(CMAKE_CXX_CPPCHECK "cppcheck")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
add_executable(aoc19 test.cpp ${SOURCES} common.cpp common.h)
# Microsoft.GSL::GSL
target_link_libraries(aoc19 PRIVATE Catch2::Catch2 absl::strings absl::status GSL fmt::fmt )
