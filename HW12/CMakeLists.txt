cmake_minimum_required (VERSION 3.12)
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

option(BUILD_TESTS "BuildTests" ON)
option(CONFIGURE_DEB_PACKAGE "Configure .deb package" ON)

set(TARGET_NAME map_reduce_project CACHE INTERNAL "Target name")
set(PROJECT_VERSION "0.0.1" CACHE INTERNAL "Version")

project (${TARGET_NAME} VERSION ${PROJECT_VERSION})
#add_executable (${TARGET_NAME} "src/main.cpp")

#set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
#set_target_properties(${TARGET_NAME}
#    PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
#    LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
#    RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin"
#)

#target_link_libraries(${TARGET_NAME} duplicate_finder_lib Boost::program_options cryptopp)
add_subdirectory(map_reduce_lib)
add_subdirectory(map_mean)
add_subdirectory(map_dispersion)
add_subdirectory(reduce_mean)
add_subdirectory(reduce_dispersion)

install(TARGETS map_mean DESTINATION .)
install(TARGETS map_dispersion DESTINATION .)
install(TARGETS reduce_mean DESTINATION .)
install(TARGETS reduce_dispersion DESTINATION .)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/source_data/AB_NYC_2019.csv DESTINATION .)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/run_scripts/run_locally.sh DESTINATION .)

set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)

# .deb package.
if(CONFIGURE_DEB_PACKAGE)
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
