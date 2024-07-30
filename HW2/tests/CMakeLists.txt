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

add_executable(UnitTests 
    ip_stream_operators_test.cpp
    test_ip_reading.cpp
)
target_link_libraries(UnitTests PUBLIC GTest::gtest_main ip)

gtest_discover_tests(UnitTests)