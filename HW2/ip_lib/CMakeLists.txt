add_library(ip STATIC
   src/ip.cpp
   include/ip_lib/ip.h
)

target_include_directories(ip
    PUBLIC include
    PRIVATE src include/ip_lib)

 set_target_properties(ip
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_features(ip PUBLIC cxx_std_20)