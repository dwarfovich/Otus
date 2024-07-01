cmake_minimum_required(VERSION 3.12)

option(BUILD_BOOST_TEST "Build tests for Boost.Tests")
option(SETUP_DEB_PACKAGE "Setup .deb package")

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(hello_project VERSION ${PROJECT_VERSION})
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)

add_executable(hello_project ${SOURCE_FILES})
add_library(hello_project_lib lib.cpp)

include_directories("${PROJECT_BINARY_DIR}/configured_files")
configure_file(project_meta_data.h.in configured_files/project_meta_data.h)

target_link_libraries(hello_project PRIVATE
    hello_project_lib
)

if(BUILD_BOOST_TEST)
    find_package(Boost COMPONENTS REQUIRED unit_test_framework)
    add_executable(test_version test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )

    target_link_libraries(test_version
        ${Boost_LIBRARIES}
        hello_project_lib
    )

    enable_testing()
    add_test(test_version test_version)
endif()

if(SETUP_DEB_PACKAGE)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGE_NAME "HelloTestPackage")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_CONTACT ouncetim@gmail.com)
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/deb")
    include(CPack)
endif()

install(TARGETS hello_project RUNTIME DESTINATION bin)

