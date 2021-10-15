cmake_minimum_required(VERSION 3.0.0)
project(pathfinder-apps)

add_executable(pathfind pf.cc)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(pathfind PRIVATE a-star maze benchmark profiler)
else()
    target_link_libraries(pathfind PRIVATE a-star maze benchmark)
endif()

find_package(FLTK CONFIG REQUIRED)

add_executable(gmaze gmaze.cc)

target_link_libraries(gmaze PRIVATE maze a-star benchmark fltk)

add_executable(cmaze cmaze.cc)
target_link_libraries(cmaze PRIVATE maze)
