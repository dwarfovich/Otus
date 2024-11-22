set(LIB_NAME db_lib CACHE INTERNAL "Library name")
add_library(${LIB_NAME} STATIC
   "src/lib.cpp"
    "include/db/database.h" "include/db/database_error.h")

target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE src include/db)

if (POLICY CMP0167 )
  cmake_policy(SET CMP0167  NEW)
endif()
set(Boost_USE_MULTITHREADED ON)
if(MSVC)
    add_definitions("-DHAS_BOOST -D_WIN32_WINNT=0x0601")
endif()
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.74 COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${LIB_NAME} Boost::thread GTest::gtest_main)

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
