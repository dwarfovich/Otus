add_library(allocator_lib STATIC
   "src/chunk_memory_manager.cpp"
   include/allocator/chunk_memory_manager.h
 "include/allocator/memory_bank.h" "include/allocator/memory_manager_allocator.h" "src/memory_manager_allocator.cpp")

target_include_directories(allocator_lib
    PUBLIC include
    PRIVATE src include/allocator)

set_target_properties(allocator_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(allocator_lib PUBLIC cxx_std_20)