#pragma once

#include <cinttypes>

namespace sokoban {

    enum class GameState : uint8_t {
        InProgress,
        Won,
        Lost
    };

}