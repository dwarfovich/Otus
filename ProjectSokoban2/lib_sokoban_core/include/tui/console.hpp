#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include "console_win.hpp"
#else
    #include "console_linux.hpp"
#endif