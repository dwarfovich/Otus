#pragma once

#include "sokoban_core/game.hpp"
#include "sokoban_core/game_action.hpp"

namespace sokoban {
namespace sbg {

class BaseGame : public Game
{
public:
    void processAction(const GameAction& action)    ;

private:


};

} // namespace sbg
} // namespace sokoban