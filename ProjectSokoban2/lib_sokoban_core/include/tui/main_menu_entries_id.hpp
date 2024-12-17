#pragma once

#include <cinttypes>

namespace sokoban {
namespace tui {

enum class MainMenuEntriesId : uint8_t
{
    Play,
    LoadGame,
    LoadMod,
    PlayLog,
    Exit,
    idsCount
};

}
} // namespace sokoban