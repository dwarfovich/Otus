set(LIB_NAME map_reduce_lib CACHE INTERNAL "Library name")
add_library(${LIB_NAME} STATIC
   "src/lib.cpp"
    "include/map_reduce/data_extractor.h"  "include/map_reduce/map_functions.h" "include/map_reduce/reduce_functions.h")

target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE src include/map_reduce)

target_link_libraries(${LIB_NAME})
set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)

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
