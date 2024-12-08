#pragma once

#include <utility>
#include <memory>

namespace sokoban {

class Command;

class Game
{
public:
    Game()          = default;
    virtual ~Game() = default;

    //virtual std::pair<bool, bool> executeCommand(const std::shared_ptr<Command>& command) = 0;
};

} // namespace sokoban