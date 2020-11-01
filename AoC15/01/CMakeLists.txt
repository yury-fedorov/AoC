cmake_minimum_required(VERSION 3.17)
project(aoc15)

set(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE( Boost 1.73 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(aoc15 day01.cpp)

TARGET_LINK_LIBRARIES( aoc15 LINK_PUBLIC ${Boost_LIBRARIES} )