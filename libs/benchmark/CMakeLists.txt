project (benchmark)

# Header only library, therefore INTERFACE
add_library(benchmark INTERFACE)

# INTERFACE targets only have INTERFACE properties
target_include_directories(benchmark INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})