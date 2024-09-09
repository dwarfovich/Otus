cmake_minimum_required (VERSION 3.12)

option(BUILD_TESTS "BuildTests" ON)
option(BUILD_BENCHMARK "BuildBenchmark" OFF)
set(PROJECT_VERSION "0.0.3" CACHE INTERNAL "Version")
project (allocator VERSION ${PROJECT_VERSION})

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib")

add_subdirectory(allocator_driver)
add_subdirectory(allocator_lib)

# deb-package.
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "allocator_driver")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT ouncetim@gmail.com)
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
include(CPack)

# Tests.
enable_testing()
add_subdirectory(tests)

if(BuildBenchmark)
    add_subdirectory(benchmark)
endif()