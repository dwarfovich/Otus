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

# set(TestsToRun
#   ip_read_operator_test.cpp
#   test_ip_reading.cpp
# )

include(GoogleTest)

add_executable(UnitTests 
    ip_read_operator_test.cpp
    test_ip_reading.cpp
)

target_link_libraries(UnitTests PUBLIC GTest::gtest_main ip)

gtest_discover_tests(UnitTests)

# create_test_sourcelist (Tests CppTests.cpp ${TestsToRun})
# add_executable (CppTests ${Tests})
# target_include_directories(CppTests  PRIVATE "include")
# set_property(TARGET CppTests PROPERTY CXX_STANDARD 20)
# target_link_libraries(CppTests GTest::gtest_main)
# #add_executable(ip_read_operator_test "ip_read_operator_test.cpp" "../src/ip.cpp" "../include/ip.h")
# foreach (test ${TestsToRun})
#     get_filename_component (TName ${test} NAME_WE)
#     add_test (NAME ${TName} COMMAND CppTests ${TName})
#     set_target_properties(CppTests
#         PROPERTIES
#         ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#         LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#         RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#     )
#     #gtest_discover_tests(${test})
# endforeach ()
