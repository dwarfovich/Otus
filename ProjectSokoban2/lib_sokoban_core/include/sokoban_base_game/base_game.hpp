#pragma once

#include "sokoban_core/game.hpp"
#include "sokoban_core/game_action.hpp"

namespace sokoban {
namespace sbg {

class BaseGame : public Game
{
public:
    BaseGame(LevelMap::TileMap map):Game{map}{}

private:


};

} // namespace sbg
} // namespace sokoban