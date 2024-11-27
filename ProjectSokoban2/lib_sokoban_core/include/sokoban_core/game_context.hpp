#pragma once

#include "game_object_factory.hpp"
#include "tile_factory.hpp"
#include "game_action.hpp"

namespace sokoban {

class GameContext
{
public:
    void setTileFactory(std::unique_ptr<TileFactory> factory){
        tileFactory_ = std::move(factory);
    } 

private:
    std::unique_ptr<GameObjectFactory> objectFactory_ = nullptr;
    std::unique_ptr<TileFactory>       tileFactory_   = nullptr;
    std::unique_ptr<GameAction>       actionFactory_   = nullptr;
};

} // namespace sc