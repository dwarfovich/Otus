add_library(allocator_lib STATIC
   "src/chunk_memory_manager.cpp"
   "src/memory_manager_allocator.cpp"
   "include/allocator/chunk_memory_manager.h"
   "include/allocator/memory_bank.h"
   "include/allocator/memory_manager_allocator.h")

target_include_directories(allocator_lib
    PUBLIC include
    PRIVATE src include/allocator)

set_target_properties(allocator_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(allocator_lib PUBLIC cxx_std_20)

#include(FetchContent)
#if (POLICY CMP0135)
#  cmake_policy(SET CMP0135 NEW)
#endif()
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
#)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

#include(GoogleTest)

target_link_libraries(allocator_lib PUBLIC GTest::gtest_main)