cmake_minimum_required (VERSION 3.12)
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

option(BUILD_TESTS "BuildTests" OFF)
option(CONFIGURE_DEB_PACKAGE "Configure .deb package" OFF)

set(TARGET_NAME Sokoban2 CACHE INTERNAL "Target name")
set(PROJECT_VERSION "0.0.1" CACHE INTERNAL "Version")

project (${TARGET_NAME} VERSION ${PROJECT_VERSION})

add_subdirectory(lib_sokoban_core)
add_subdirectory(sokoban_driver)

include_directories(${PROJECT_SOURCE_DIR}/submodules/json/include/)
add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/json/ NlohmannJson)
include_directories(${PROJECT_SOURCE_DIR}/submodules/json/include/)
#add_custom_command(
#    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Addons/Core/
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sokoban_base_data/ ${CMAKE_CURRENT_BINARY_DIR}/Addons/Core/
#    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/sokoban_base_data/)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/Addons/Core/tiles_id.json
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/sokoban_base_data/tiles_id.json
    COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/sokoban_base_data/tiles_id.json
        ${CMAKE_CURRENT_BINARY_DIR}/Addons/Core/tiles_id.json
)
add_custom_target(copy DEPENDS  ${CMAKE_CURRENT_BINARY_DIR}/Addons/Core/tiles_id.json)
#add_custom_target(${TARGET_NAME} DEPENDS  ${CMAKE_CURRENT_BINARY_DIR}/Addons/Core/tiles_id.json)

# .deb package.
if(CONFIGURE_DEB_PACKAGE)
    set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGE_NAME "map_reduce")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_CONTACT "ouncetim@gmail.com")
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
    set(CPACK_SET_DESTDIR true)
    set(CPACK_INSTALL_PREFIX /opt/map_reduce)
    include(CPack)
endif()

# Tests.
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
