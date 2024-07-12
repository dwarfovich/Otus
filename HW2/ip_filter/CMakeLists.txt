# CMakeList.txt : CMake project for ip_filter, include source and define
# project specific logic here.
#

# Source to this project's executable.
add_executable (ip_filter "main.cpp"  "ip.h" "ip.cpp")

set_property(TARGET ip_filter PROPERTY CXX_STANDARD 20)

# Tests and install targets.
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "ip_filter")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT ouncetim@gmail.com)
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/deb")
include(CPack)

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

add_executable(ip_read_operator_test "tests/ip_read_operator_test.cpp" "ip.h" "ip.cpp")

set_property(TARGET ip_read_operator_test PROPERTY CXX_STANDARD 20)
target_link_libraries(
  ip_read_operator_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ip_read_operator_test)