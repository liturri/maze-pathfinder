project(gzstream)

add_library(gzstream gzstream.cc)
target_link_libraries(gzstream PRIVATE z)
target_include_directories(gzstream PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})