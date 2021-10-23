cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_COMPILER "/usr/bin/clang-12")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# The project name
project(maze-pathfinder VERSION 0.1.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -Werror -g3 -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -Werror -g3")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 -Wextra -Wall")
# set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -pg")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE} with ${CMAKE_CXX_COMPILER}")

add_subdirectory(src)
add_subdirectory(libs/AStar)
add_subdirectory(libs/maze)
add_subdirectory(libs/benchmark)
add_subdirectory(libs/gzstream)