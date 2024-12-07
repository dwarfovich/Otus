#pragma once

#define NOMINMAX
#include <Windows.h>

namespace sokoban{
    namespace tui{
    
        inline void clearConsole() {
            system("cls");
        }

    }
}