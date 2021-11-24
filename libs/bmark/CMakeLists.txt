project (bmark)

# Header only library, therefore INTERFACE
add_library(bmark INTERFACE)

# INTERFACE targets only have INTERFACE properties
target_include_directories(bmark INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})