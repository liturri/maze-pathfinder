project(maze)

add_library(maze maze.cc)
target_include_directories(maze PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})