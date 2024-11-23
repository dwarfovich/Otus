#pragma once

#include <iostream>

class DebugOut
{
public:
    template<typename T>
    DebugOut& operator<<(const T& data)
    {
#ifdef _DEBUG
        std::cout << data;
#endif // _DEBUG

        return *this;
    }
};

static inline DebugOut g_debugOut;