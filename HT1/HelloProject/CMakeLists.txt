cmake_minimum_required(VERSION 3.12)
project(hello_project)

option(USE_BOOST_TEST "Build tests for Boost.Tests" ON)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)

add_executable(hello_project ${SOURCE_FILES} "HelloClass.h" "HelloClass.cpp")
add_library(hello_project_lib Lib.cpp)

include_directories("${PROJECT_BINARY_DIR}")

configure_file(Version.h.in version.h)

target_link_libraries(hello_project PRIVATE
    hello_project_lib
)

if(USE_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_version TestVersion.cpp)

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

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

#install(TARGETS hello_world RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT ouncetim@gmail.com)
include(CPack)
