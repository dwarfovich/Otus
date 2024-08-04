cmake_minimum_required (VERSION 3.12)

option(BUILD_TESTS "BuildTests" ON)
set(PROJECT_VERSION "0.0.3" CACHE INTERNAL "Version")
project (ip_filter_project VERSION ${PROJECT_VERSION})

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_subdirectory(ip_filter)
add_subdirectory(ip_lib)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()