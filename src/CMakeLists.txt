cmake_minimum_required(VERSION 3.0.0)
project(pathfinder-apps)

add_executable(pathfind pf.cc app_config.cc seeds.cc)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(pathfind PRIVATE a-star pthread maze bmark argparse profiler)
elseif(CMAKE_BUILD_TYPE STREQUAL "Static")
    target_link_libraries(pathfind PRIVATE a-star maze bmark argparse -static -lrt -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive)
else()
    target_link_libraries(pathfind PRIVATE a-star pthread maze bmark argparse )
endif()


find_package(FLTK CONFIG )

if (FLTK_FOUND)
add_executable(gmaze gmaze.cc app_config.cc)

target_link_libraries(gmaze PRIVATE maze a-star argparse bmark fltk)
endif()

add_executable(cmaze cmaze.cc app_config.cc)
target_link_libraries(cmaze PRIVATE maze argparse bmark)

add_executable(seed_db seed_db.cc seeds.cc)
target_link_libraries(seed_db PRIVATE gzstream bmark ) # profiler

set(BENCHMARK_DOWNLOAD_DEPENDENCIES on)
add_subdirectory(../../benchmark/ bench-build)

add_executable(pf_bench pf_bench.cc)
target_link_libraries(pf_bench PRIVATE a-star maze benchmark::benchmark)