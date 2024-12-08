#pragma once

#include "windows.hpp"

#include <iostream>

namespace sokoban {

class System
{
public:
    System(const System&)            = delete;
    System& operator=(const System&) = delete;

    // static System& instance()
    //{
    //     static System s;
    //     return s;
    // }

    /*static bool initialize()
    {
        if (instance().initialized_) {
            return true;
        }

        instance().stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        instance().stdIn  = GetStdHandle(STD_INPUT_HANDLE);
        if (instance().stdOut == 0 || instance().stdOut == INVALID_HANDLE_VALUE || instance().stdIn == 0
            || instance().stdIn == INVALID_HANDLE_VALUE) {
            return false;
        }

        instance().initialized_ = true;

        return true;
    }*/

    HANDLE outputHandle() { return stdOutHandle_; }
    HANDLE inputHandle() { return stdInHandle_; }

public: // methods
    System()
    {
        stdOutHandle_ = GetStdHandle(STD_OUTPUT_HANDLE);
        stdInHandle_  = GetStdHandle(STD_INPUT_HANDLE);
        if (stdOutHandle_ == 0 || stdOutHandle_ == INVALID_HANDLE_VALUE || stdInHandle_ == 0
            || stdInHandle_ == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to initialize handles");
        }
    }
    ~System() {}

private: // data
    bool   initialized_  = false;
    HANDLE stdOutHandle_ = 0;
    HANDLE stdInHandle_  = 0;
};

} // namespace sokoban