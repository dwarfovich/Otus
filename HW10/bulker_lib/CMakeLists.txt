set(LIB_NAME bulker_lib CACHE INTERNAL "Library name")

add_library(${LIB_NAME} 
   "src/lib.cpp"
   "include/bulker/command_factory.h"
   "include/bulker/command_identifier.h"
   "include/bulker/command.h"
   "include/bulker/command_parser.h"
   "include/bulker/bulker_command_factory.h"
   "include/bulker/bulker_command_parser.h"
   "include/bulker/bulker_command_executor.h"
   "include/bulker/command_block.h"
   "include/bulker/bulker_command.h"
   "include/bulker/loggable.h" 
   "include/bulker/logger.h" 
   "include/bulker/bulker_logger.h" 
   "include/bulker/bulker_dummy_logger.h"
   "include/bulker/dummy_stream.h"
   "include/bulker_sl/async.cpp"
 "include/bulker_sl/command_executor_context.h" "include/bulker_sl/threaded_bulker_logger.h" "include/bulker_sl/thread_safe_cout.h" "include/bulker_sl/command_executor_context_hasher.h"
 "include/bulker_net/boost_aliases.h"
 "include/bulker_net/default_port.h"
 "include/bulker_net/port_number.h"
 "include/bulker_net/request_processor.h"
 "include/bulker_net/server.h"
 "include/bulker_net/session.h"
 "include/bulker_net/session.cpp"
 "include/bulker_net/debug_message.h"
)

target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE src include/bulker)

target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE src include/bulker_sl)

target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE src include/bulker_net)

set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)

if (WIN32)
    add_definitions(-DDLL_EXPORT)
endif()

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
target_link_libraries(${LIB_NAME} Boost::thread)

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