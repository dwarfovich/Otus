#pragma once

#include <Windows.h>

namespace sokoban {
namespace tui {

enum class Key : unsigned
{
    invalidKey,
    digit1,
    digit2,
    digit3,
    digit4,
    digit5,
    a,
    s,
    d,
    w,
    leftArrow,
    rightArrow,
    upArrow,
    downArrow,
    esc,

};

Key fromVirtualWord(WORD c)
{
    switch (c) {
        case 49: return Key::digit1;
        case 50: return Key::digit2;
        case 51: return Key::digit3;
        case 52: return Key::digit4;
        case 53: return Key::digit5;
        case 65: return Key::a;
        case 83: return Key::s;
        case 68: return Key::d;
        case 87: return Key::w;
        case 37: return Key::leftArrow;
        case 39: return Key::rightArrow;
        case 38: return Key::upArrow;
        case 40: return Key::downArrow;
        case 27: return Key::esc;
        default: return Key::invalidKey;
    }
}

} // namespace tui
} // namespace sokoban