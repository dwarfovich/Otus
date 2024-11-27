#pragma once

#include "game_object_factory.hpp"
#include "tile_factory.hpp"

namespace sokoban {

class GameContext
{
public:
private:
    std::unique_ptr<GameObjectFactory> objectFactory_ = nullptr;
    std::unique_ptr<TileFactory>       tileFactory_   = nullptr;
};

} // namespace sc