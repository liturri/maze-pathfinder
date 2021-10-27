cmake_minimum_required(VERSION 3.0.0)
project(pathfinder-apps)

add_executable(pathfind pf.cc seeds.cc)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(pathfind PRIVATE a-star pthread maze benchmark profiler)
elseif(CMAKE_BUILD_TYPE STREQUAL "Static")
    target_link_libraries(pathfind PRIVATE a-star maze benchmark -static -lrt -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive)
else()
    target_link_libraries(pathfind PRIVATE a-star pthread maze benchmark)
endif()


find_package(FLTK CONFIG )

add_executable(gmaze gmaze.cc)

target_link_libraries(gmaze PRIVATE maze a-star benchmark fltk)

add_executable(cmaze cmaze.cc)
target_link_libraries(cmaze PRIVATE maze)

add_executable(seed_db seed_db.cc seeds.cc)
target_link_libraries(seed_db PRIVATE gzstream benchmark ) # profiler
