#pragma once

#include "base_tile_factory.hpp"

#include "../sokoban_core/game_context.hpp"
#include "../sokoban_core/default_paths.hpp"

#include <memory>

namespace sokoban{
namespace sbg{



std::unique_ptr<GameContext> createContext(){
    /*
    std::unique_ptr<GameObjectFactory> objectFactory_ = nullptr;
    std::unique_ptr<TileFactory>       tileFactory_   = nullptr;
    std::unique_ptr<GameAction>       actionFactory_   = nullptr;
    */
    auto context = std::make_unique<GameContext>();
    //context->setTileFactory(std::make_unique<BaseTileFactory>(default_paths::coreDataFolder / "tiles_id.json"));

    return context;
}
}

}