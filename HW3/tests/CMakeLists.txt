include(FetchContent)
if (POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(unit_tests 
    #"chunk_memory_manager_tests.cpp"
    #"memory_manager_allocator_tests.cpp"
    "forward_list_test.cpp")
target_link_libraries(unit_tests PUBLIC GTest::gtest_main allocator_lib cc_lib)

gtest_discover_tests(unit_tests)