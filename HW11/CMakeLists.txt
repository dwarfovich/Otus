﻿cmake_minimum_required (VERSION 3.12)
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

option(BUILD_TESTS "BuildTests" ON)
option(CONFIGURE_DEB_PACKAGE "Configure .deb package" ON)

#set(TARGET_NAME  CACHE INTERNAL "Target name")
set(PROJECT_VERSION "0.0.1" CACHE INTERNAL "Version")

project (join_server VERSION ${PROJECT_VERSION})
#add_executable (${TARGET_NAME} "src/main.cpp" "src_server/main.cpp")
#set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
#set_target_properties(${TARGET_NAME}
#    PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
#    LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
#    RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin"
#)
set(Boost_NO_WARN_NEW_VERSIONS 1)
if(MSVC)
    add_definitions("-DHAS_BOOST -D_WIN32_WINNT=0x0601")
endif()
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.74 REQUIRED)

#target_link_libraries(${TARGET_NAME} db_lib)
add_subdirectory(common_aux_lib)
add_subdirectory(db_lib)
add_subdirectory(src_client)
add_subdirectory(src_server)

install(TARGETS join_server DESTINATION .)

# .deb package.
if(CONFIGURE_DEB_PACKAGE)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGE_NAME ${TARGET_NAME})
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_PACKAGE_CONTACT "ouncetim@gmail.com")
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
    set(CPACK_SET_DESTDIR true)
    set(CPACK_INSTALL_PREFIX /opt/join_server)
    include(CPack)
endif()

# Tests.
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
