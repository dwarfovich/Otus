cmake_minimum_required (VERSION 3.12)
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(TARGET_NAME fashion_mnist CACHE INTERNAL "Target name")
set(PROJECT_VERSION "0.0.1" CACHE INTERNAL "Version")

project (${TARGET_NAME} VERSION ${PROJECT_VERSION})
add_executable (${TARGET_NAME} "main.cpp" "predictor.h" "linreg_predictor.h" "linreg_predictor.cpp")
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
set_target_properties(${TARGET_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin"
)

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
target_link_libraries(fashion_mnist Boost::thread)


#target_link_libraries(${TARGET_NAME} map_reduce_lib)

add_custom_command(TARGET fashion_mnist POST_BUILD        
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different  
                   ${CMAKE_SOURCE_DIR}/hw_data/test_data_logreg.txt
                   ${CMAKE_BINARY_DIR}/bin/test_data_logreg.txt)            
              
                   add_custom_command(TARGET fashion_mnist POST_BUILD        
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different  
                   ${CMAKE_SOURCE_DIR}/hw_data/logreg_coef.txt
                   ${CMAKE_BINARY_DIR}/bin/logreg_coef.txt)     

                   add_custom_command(TARGET fashion_mnist POST_BUILD        
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different  
                   ${CMAKE_SOURCE_DIR}/hw_data/test.csv
                   ${CMAKE_BINARY_DIR}/bin/test.csv)     