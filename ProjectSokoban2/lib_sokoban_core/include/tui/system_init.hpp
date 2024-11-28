#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#include <iostream>

namespace sokoban {
namespace tui {

struct System
{
    System(const System&)            = delete;
    System& operator=(const System&) = delete;

    static System& instance()
    {
        static System s;
        return s;
    }

    static bool initialize()
    {
        if (instance().initialized_) {
            return true;
        }
        //AllocConsole();
        instance().stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        instance().stdIn  = GetStdHandle(STD_INPUT_HANDLE);
        std::cout << instance().stdOut << " "
                  << instance().stdIn << '\n';  
        if (instance().stdOut == 0 || instance().stdOut == INVALID_HANDLE_VALUE || instance().stdIn == 0
            || instance().stdIn == INVALID_HANDLE_VALUE) {
            return false;
        }

        instance().initialized_ = true;

        return true;
    }

    static HANDLE outputHandle() { return instance().stdOut; }
    static HANDLE inputHandle() { return instance().stdIn; }

private: // methods
    System() {}
    ~System() {}

private: // data
    bool   initialized_ = false;
    HANDLE stdOut       = 0;
    HANDLE stdIn        = 0;
};

} // namespace tui
} // namespace sokoban

#endif //
