#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include "keyboard_win.hpp"
#else
#include "keyboard_linux.hpp"
#endif

