#pragma once

#include "windows.hpp"

#include <iostream>

namespace sokoban {

class System
{
public:
    System(const System&)            = delete;
    System& operator=(const System&) = delete;

    //static System& instance()
    //{
    //    static System s;
    //    return s;
    //}

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

     HANDLE outputHandle() { return stdOut; }
     HANDLE inputHandle() { return stdIn; }

public: // methods
    System()
    {
        stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        stdIn  = GetStdHandle(STD_INPUT_HANDLE);
        if (stdOut == 0 || stdOut == INVALID_HANDLE_VALUE || stdIn == 0
            || stdIn == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to initialize handles");
        }
    }
    ~System() {}

private: // data
    bool   initialized_ = false;
    HANDLE stdOut       = 0;
    HANDLE stdIn        = 0;

};

} // namespace sokoban