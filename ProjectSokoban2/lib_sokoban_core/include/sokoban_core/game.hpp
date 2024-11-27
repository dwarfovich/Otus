#pragma once

#include "game_context.hpp"
namespace sokoban {

    class Game
{
public:
    virtual ~Game() = default;

    void initialize(const std::shared_ptr<GameContext>& context) { context_ = context; }

private:
    std::shared_ptr<GameContext> context_;
};

} // namespace sc