#pragma once

#include <cinttypes>

namespace sokoban {
namespace tui {

enum class MainMenuEntriesId : uint8_t
{
    Play,
    LoadGame,
    LoadMod,
    Options,
    Exit,
    idsCount
};

}
} // namespace sokoban