set(LIB_NAME sokoban_core_lib CACHE INTERNAL "Library name")
add_library(${LIB_NAME} STATIC
   "src/lib.cpp"
      "include/sokoban_core/game_object_id.hpp" "include/sokoban_core/game_action.hpp" "include/sokoban_core/default_paths.hpp" "include/sokoban_core/multimodal_interface.hpp" "include/sokoban_core/session_context.hpp" "include/tui/menu.hpp" "include/tui/menu_entry.hpp" "include/tui/keyboard_win.hpp" "include/tui/tui.h")

target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE src include/sokoban_core include/sokoban_base_game include/tui)


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

target_link_libraries(${LIB_NAME} Boost::thread )

set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)

#add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/json/ NlohmannJson)
include_directories(${PROJECT_SOURCE_DIR}/include/)
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/submodules/json/single_include/)

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
