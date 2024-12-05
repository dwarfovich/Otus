#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include "tui_win.hpp"
#else
    #include "tui_linux.hpp"
#endif
