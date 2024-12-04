#pragma once

// #include "action_result.hpp"
#include "command.hpp"

#include <memory>
#include <filesystem>

namespace sokoban {
class GameContext;
class MultimodalInterface;
class ActionResult;
class LevelMap;
} // namespace sokoban

namespace sokoban {

class SessionContext
{
public:
    virtual MultimodalInterface& multimodalInterface()                                   = 0;
    virtual bool                 executeCommand(const std::shared_ptr<Command>& command) = 0;
    virtual void startGame() = 0;
};

} // namespace sokoban