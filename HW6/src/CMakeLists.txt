add_executable(allocator_driver main.cpp)
target_link_libraries(allocator_driver PUBLIC allocator_lib cc_lib)

set_target_properties(allocator_driver
PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
#string(REGEX REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
if(MSVC)
  target_compile_options(allocator_driver PRIVATE /W4 /WX)
else()
  target_compile_options(allocator_driver PRIVATE -Wall -Wextra -Wpedantic)
endif()

# deb-package.
# set(CPACK_GENERATOR DEB)
# set(CPACK_PACKAGE_NAME "ip_filter")
# set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
# set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
# set(CPACK_PACKAGE_ARCHITECTURE "amd64")
# set(CPACK_PACKAGE_CONTACT ouncetim@gmail.com)
# set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
# include(CPack)