add_library(allocator_lib STATIC
   "src/lib.cpp"
   "include/allocator/chunk_memory_manager.h"
   "include/allocator/memory_bank.h"
   "include/allocator/memory_manager_allocator.h"
)

target_include_directories(allocator_lib
    PUBLIC include
    PRIVATE src include/allocator)

set_target_properties(allocator_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(allocator_lib PUBLIC cxx_std_20)
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

target_link_libraries(allocator_lib PUBLIC GTest::gtest_main)