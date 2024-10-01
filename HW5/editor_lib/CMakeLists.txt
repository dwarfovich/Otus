add_library(editor_lib STATIC
   "src/lib.cpp"
   "include/editor/document.h"
 "include/editor/graphics_item.h" "include/editor/color.h" "include/editor/point.h" "include/editor/rect.h"  "include/editor/view.h" "include/editor/view_painter.h" "include/editor/primitive_items.h"  "include/editor/dummy_converter.h"   "include/editor/controller.h" "include/editor/command.h" "include/editor/widget.h" "include/editor/push_button.h" "include/editor/add_graphics_item_command.h" "include/editor/main_window.h")

target_include_directories(editor_lib
    PUBLIC include
    PRIVATE src include/cc)

set_target_properties(editor_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_compile_features(editor_lib PUBLIC cxx_std_20)
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