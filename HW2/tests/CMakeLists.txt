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

add_executable(ip_read_operator_test "ip_read_operator_test.cpp" "../src/ip.cpp" "../include/ip.h")

target_include_directories(ip_read_operator_test PRIVATE "include")

set_target_properties(ip_read_operator_test
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
set_property(TARGET ip_read_operator_test PROPERTY CXX_STANDARD 20)
target_link_libraries(
  ip_read_operator_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ip_read_operator_test)