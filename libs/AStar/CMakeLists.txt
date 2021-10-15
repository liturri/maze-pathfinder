project(a-star)

add_library(a-star STATIC AStar.cc)

target_include_directories(a-star PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})