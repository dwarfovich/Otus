set(LIB_NAME base_game_lib CACHE INTERNAL "Library name")
add_library(base_game_lib SHARED
   "src/lib.cpp"
   "include/base_game_object_factory.hpp"
      "include/base_game_object_factory.cpp"
      "include/base_action_factory.hpp"
      "include/base_game.hpp"
      "include/base_game_action.hpp"
      "include/base_game_action.cpp"
      "include/base_multimodal_interface.hpp"
      "include/base_session_context.hpp"
      "include/base_tile_factory.hpp"
      "include/objects_to_symbol_hasher.hpp"
 "src/mod.cpp")

target_include_directories(base_game_lib
    PUBLIC include
    PRIVATE src )
target_link_libraries(base_game_lib sokoban_core_lib)


if (POLICY CMP0167 )
  cmake_policy(SET CMP0167  NEW)
endif()
set(Boost_USE_MULTITHREADED ON)
if(MSVC)
    add_definitions("-DHAS_BOOST -D_WIN32_WINNT=0x0601")
endif()
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.74 COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

target_link_libraries(base_game_lib Boost::filesystem)

set_target_properties(base_game_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(base_game_lib PUBLIC cxx_std_20)

#add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/json/ NlohmannJson)
#include_directories(${PROJECT_SOURCE_DIR}/include/)
#target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/submodules/json/single_include/)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
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
