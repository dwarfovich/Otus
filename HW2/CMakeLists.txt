cmake_minimum_required (VERSION 3.12)

option(BUILD_TESTS "BuildTests" OFF)
set(PROJECT_VERSION "0.0.3" CACHE INTERNAL "Version")

project ("ip_filter" VERSION ${PROJECT_VERSION})
add_executable (ip_filter "src/main.cpp" "src/ip.cpp" "include/ip.h")
include_directories("include")

set_property(TARGET ip_filter PROPERTY CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set_target_properties(ip_filter
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# deb-package.
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "ip_filter")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT ouncetim@gmail.com)
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
include(CPack)

# Tests.
if(BuildTests)
    add_subdirectory(tests)
endif()