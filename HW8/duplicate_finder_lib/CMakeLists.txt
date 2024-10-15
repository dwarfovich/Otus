set(LIB_NAME duplicate_finder_lib CACHE INTERNAL "Library name")
add_library(${LIB_NAME} STATIC
   "src/lib.cpp" "include/duplicate_finder/duplicate_finder.h" "include/duplicate_finder/finder_task.h" "include/duplicate_finder/paths_vector.h" "include/duplicate_finder/strings_vector.h" "include/duplicate_finder/digester.h")

target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE src include/duplicate_finder)



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