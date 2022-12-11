cmake_minimum_required(VERSION 3.17) # 3.20 for CXX_STANDARD 23, 3.18 for Ubuntu, 3.17 for Windows
project(aoc22)

set(CMAKE_CXX_STANDARD 20) # 3.20 for CXX_STANDARD 23, latest Ubuntu 22.10 3.24
set(ABSL_PROPAGATE_CXX_STD, ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # TODO - lots of warnings and all warnings as errors
    # removed to make almost clean Abseil: -Wall -Wextra -Werror -pedantic
    # -fexperimental-library - to be able to use experimental features like ranges/views
    # -Wno-unused-but-set-variable - to be able to compile benchmark
    add_compile_options(-Wno-deprecated -fno-exceptions -Wno-unused-but-set-variable -fno-rtti)
endif()

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address") #memory - cannot together with address #undefined,dataflow
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

include(FetchContent)

FetchContent_Declare(absl
    GIT_REPOSITORY "https://github.com/abseil/abseil-cpp.git"
    GIT_TAG master ) # as ie bug in low_level_hash was fixed

FetchContent_Declare(gtest
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG main )

FetchContent_Declare(benchmark
    GIT_REPOSITORY "https://github.com/google/benchmark"
    GIT_TAG main ) # benchmark-src/test/options_test.cc:70:10: error: variable 'actual_iterations' set but not used [-Werror,-Wunused-but-set-variable]
                     # size_t actual_iterations = 0;

FetchContent_Declare(re2
    GIT_REPOSITORY "https://github.com/google/re2"
    GIT_TAG main ) 

FetchContent_MakeAvailable(absl)
FetchContent_MakeAvailable(gtest)
FetchContent_MakeAvailable(benchmark)
FetchContent_MakeAvailable(re2)

file( GLOB SOURCES day*.cpp )
# set(CMAKE_CXX_CPPCHECK "cppcheck")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
add_executable(aoc22 test.cpp ${SOURCES}  )

target_link_libraries(aoc22 PRIVATE absl::flat_hash_set absl::strings gtest re2 absl::statusor benchmark)
