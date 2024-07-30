add_library(ip STATIC
   src/ip.cpp
   include/ip_lib/ip.h
)

target_include_directories(ip
    PUBLIC include
    PRIVATE src include/ip_lib)

set_target_properties(ip PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(ip PUBLIC cxx_std_20)