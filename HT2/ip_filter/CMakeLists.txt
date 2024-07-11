# CMakeList.txt : CMake project for HT2, include source and define
# project specific logic here.
#

# Source to this project's executable.
add_executable (ip_filter "main.cpp"  "ip.h" "ip.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ip_filter PROPERTY CXX_STANDARD 20)
endif()

# Tests and install targets.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  hello_test
  hello_test.cc
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)