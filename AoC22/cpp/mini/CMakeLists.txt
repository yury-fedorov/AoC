# Used for mini project
cmake_minimum_required(VERSION 3.20) # 3.20 for CXX_STANDARD 23, 3.18 for Ubuntu, 3.17 for Windows
project(aoc22-mini)

set(CMAKE_CXX_STANDARD 23) # 3.20 for CXX_STANDARD 23, latest Ubuntu 22.10 3.24

if (MSVC)
    # warning level 4 and all warnings as errors
    # add_compile_options(/W4 /WX)
else()
    # TODO - lots of warnings and all warnings as errors
    # removed to make almost clean Abseil: -Wall -Wextra -Werror -pedantic
    # -fexperimental-library - to be able to use experimental features like ranges/views    
    add_compile_options(-Wno-deprecated -fno-exceptions -fno-rtti)    

    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address") #memory - cannot together with address #undefined,dataflow
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # Only for Clang we add experimental for ranges/views as GCC has it already
    # add_compile_options(-fexperimental-library) 
endif()

# include(FetchContent)
# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.7.1  # the latest release of Catch2 v3
# )
# FetchContent_MakeAvailable(Catch2)

file( GLOB SOURCES ../day16.cpp ../day19.cpp ../common.cpp)

# set(CMAKE_CXX_CPPCHECK "cppcheck")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
add_executable(aoc22-mini main.cpp ${SOURCES})
target_include_directories(aoc22-mini PRIVATE "..")
# target_link_libraries(aoc22-mini PRIVATE Catch2::Catch2)
target_link_libraries(aoc22-mini PRIVATE)
