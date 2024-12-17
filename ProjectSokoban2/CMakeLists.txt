cmake_minimum_required (VERSION 3.26)

option(CONFIGURE_DEB_PACKAGE "Configure .deb package" OFF)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS True)
set(TARGET_NAME Sokoban2 CACHE INTERNAL "Target name")
set(PROJECT_VERSION "0.0.1" CACHE INTERNAL "Version")

project (${TARGET_NAME} VERSION ${PROJECT_VERSION})

add_subdirectory(lib_sokoban_core)
add_subdirectory(lib_base_game)
add_subdirectory(sokoban_driver)

# .deb package.
if(CONFIGURE_DEB_PACKAGE)
    set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGE_NAME "sokoban2")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_CONTACT "ouncetim@gmail.com")
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
    set(CPACK_SET_DESTDIR true)
    set(CPACK_INSTALL_PREFIX /opt/map_reduce)
    include(CPack)
endif()