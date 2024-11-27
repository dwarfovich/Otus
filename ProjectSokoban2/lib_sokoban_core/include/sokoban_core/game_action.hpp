#pragma once

#include "game_context.hpp"
#include "level_map.hpp"
#include "game_object.hpp"

namespace sokoban{

    class GameAction{
    public:
        virtual ~GameAction()= default;

//        virtual bool tryPerform(GameContext& context, LevelMap& map, GameObject& object) = 0;
    };
}