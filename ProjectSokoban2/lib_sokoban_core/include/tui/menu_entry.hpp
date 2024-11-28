#pragma once

#include "string"

namespace sokoban {
namespace tui{

template<typename Enum>
class MenuEntry
{
public:
    std::string text;
    Enum        value;
};

}
}