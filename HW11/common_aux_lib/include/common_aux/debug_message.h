#pragma once

#include <iostream>

class DebugOut{
public:

    template<typename T>
    DebugOut& operator<< (const T& data){
        std::cout << data;

        return *this;
    }
};

static inline DebugOut g_debugOut;