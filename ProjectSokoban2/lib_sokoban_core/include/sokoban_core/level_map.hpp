#pragma once

#include "tile.hpp"

#include <vector>

namespace sokoban {

    class LevelMap{
    private:
        std::vector<std::vector<Tile>> map_;
    };
}