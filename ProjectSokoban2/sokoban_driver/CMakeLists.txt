cmake_minimum_required (VERSION 3.26)

set(TARGET_NAME sokoban_driver CACHE INTERNAL "Target name")
set(PROJECT_VERSION "0.0.1" CACHE INTERNAL "Version")

if (POLICY CMP0167 )
  cmake_policy(SET CMP0167  NEW)
endif()
set(Boost_USE_MULTITHREADED ON)
if(MSVC)
    add_definitions("-DHAS_BOOST -D_WIN32_WINNT=0x0601")
endif()
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.74 COMPONENTS thread filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

project (${TARGET_NAME} VERSION ${PROJECT_VERSION})
add_executable (${TARGET_NAME} "main.cpp")
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
set_target_properties(${TARGET_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(${TARGET_NAME} Boost::filesystem sokoban_core_lib base_game_lib)

install(FILES $<TARGET_RUNTIME_DLLS:sokoban_driver> TYPE BIN)

add_custom_command(TARGET sokoban_driver POST_BUILD        
                       COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different  
                       "${CMAKE_SOURCE_DIR}/sokoban_base_data/BaseGame"
                       "${CMAKE_BINARY_DIR}/bin/Mods/BaseGame")       
add_custom_command(TARGET sokoban_driver POST_BUILD        
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different   
                   "${CMAKE_SOURCE_DIR}/sokoban_base_data/players.json"
                   "${CMAKE_BINARY_DIR}/bin/players.json")  

if (WIN32)
    add_custom_command(TARGET sokoban_driver POST_BUILD        
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different  
                       "${CMAKE_SOURCE_DIR}/build/msvc-debug/lib_base_game/base_game_lib.dll"      
                       "${CMAKE_BINARY_DIR}/bin/Mods/BaseGame/mod")  
else()
    add_custom_command(TARGET sokoban_driver POST_BUILD        
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different  
                       "${CMAKE_SOURCE_DIR}/build/linux-debug/lib/libbase_game_lib.so"      
                       "${CMAKE_BINARY_DIR}/bin/Mods/BaseGame/mod")  
endif()