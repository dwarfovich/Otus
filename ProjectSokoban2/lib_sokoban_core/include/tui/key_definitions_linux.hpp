#pragma once

#include <cstdint>

namespace sokoban {
namespace tui {

inline Key fromVirtualChar(uint32_t c)
{
    switch (c) {
        case 49: return Key::digit1;
        case 50: return Key::digit2;
        case 51: return Key::digit3;
        case 52: return Key::digit4;
        case 53: return Key::digit5;
        case 97: return Key::a;
        case 115: return Key::s;
        case 100: return Key::d;
        case 119: return Key::w;
        case 4479771: return Key::leftArrow;
        case 4414235: return Key::rightArrow;
        case 4283163: return Key::upArrow;
        case 4348699: return Key::downArrow;
        case 27: return Key::esc;
        default: return Key::invalidKey;
    }
}

} // namespace tui
} // namespace sokoban