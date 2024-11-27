#pragma once

#include "game_context.hpp"

#include <memory>

namespace sokoban{

    class GameContextFactory{
    public:
        virtual ~GameContextFactory() = default;

        virtual std::unique_ptr<GameContext> create() = 0;

    };
}