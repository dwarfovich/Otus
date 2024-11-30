#pragma once

#include "game_context.hpp"
#include "level_map.hpp"

namespace sokoban {

    class Game
{
public:
    virtual ~Game() = default;

    void setMap(const LevelMap& map){
        map_ = map;
    }

protected:
    // gameState_;
    LevelMap map_;
};

} // namespace sc