cmake_minimum_required (VERSION 3.12)

option(BUILD_TESTS "BuildTests" ON)
set(TARGET_NAME print_ip CACHE INTERNAL "Target name")
set(PROJECT_VERSION "0.0.4" CACHE INTERNAL "Version")

project (${TARGET_NAME} VERSION ${PROJECT_VERSION})
add_executable (${TARGET_NAME} "src/main.cpp" "include/print_ip.h")
install(TARGETS ${TARGET_NAME} DESTINATION bin)
include_directories("include/")
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set_target_properties(${TARGET_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# deb-package.
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "print_ip")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT ouncetim@gmail.com)
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
include(CPack)

# Tests.
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()