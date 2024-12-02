#pragma once

#include "level_map.hpp"
#include "game_object.hpp"
#include "session_context.hpp"

namespace sokoban{

    class GameAction{
    public:
        virtual ~GameAction()= default;

        virtual void perform(SessionContext& context){
        }
    };
}