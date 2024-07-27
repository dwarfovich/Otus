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

enable_testing()

add_executable(sanity_check "sanity_check.cpp" "../include/print_ip.h")

set_target_properties(sanity_check
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
set_property(TARGET sanity_check PROPERTY CXX_STANDARD 20)
target_link_libraries(
  sanity_check
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sanity_check)