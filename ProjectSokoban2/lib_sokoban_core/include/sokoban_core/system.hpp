#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include "windows.hpp"
#else
    #define HANDLE int
    #include <termios.h>
#endif

#include <iostream>

namespace sokoban {

class System
{
public:
    System(const System&)            = delete;
    System& operator=(const System&) = delete;

    HANDLE outputHandle() { return stdOutHandle_; }
    HANDLE inputHandle() { return stdInHandle_; }

public: // methods
    System()
    {
#if defined(_WIN32) || defined(_WIN64)
        stdOutHandle_ = GetStdHandle(STD_OUTPUT_HANDLE);
        stdInHandle_  = GetStdHandle(STD_INPUT_HANDLE);
        if (stdOutHandle_ == 0 || stdOutHandle_ == INVALID_HANDLE_VALUE || stdInHandle_ == 0
            || stdInHandle_ == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to initialize handles");
        }
        #else
        tcgetattr(fileno(stdin), &oldSettings);
        newSettings = oldSettings;
        newSettings.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr(fileno(stdin), TCSANOW, &newSettings);
#endif
    }
    ~System() { 
        #if defined(_WIN32) || defined(_WIN64)
#else
        tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
#endif
    }

private: // data
    bool   initialized_  = false;
    HANDLE stdOutHandle_ = 0;
    HANDLE stdInHandle_  = 0;
    #if !defined(_WIN32) && !defined(_WIN64)
    termios oldSettings;
    termios newSettings;
    #endif
};

} // namespace sokoban
