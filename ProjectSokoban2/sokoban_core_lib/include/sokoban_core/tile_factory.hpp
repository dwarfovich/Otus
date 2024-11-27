#pragma once

#include "tile.hpp"

#include <memory>

namespace sokoban {

    class TileFactory{
        virtual ~TileFactory() = default;

        virtual TileUptr create() = 0;
    };

}