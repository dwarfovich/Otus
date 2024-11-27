#pragma once

#include "tile.hpp"

#include <memory>

namespace sokoban {

    class TileFactory{
    public:
        virtual ~TileFactory() = default;

        virtual TileUptr create() const = 0;
    };

}