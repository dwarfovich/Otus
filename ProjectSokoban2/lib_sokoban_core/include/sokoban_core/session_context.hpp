#pragma once

//#include "action_result.hpp"
#include "command.hpp"

#include <memory>
#include <filesystem>

namespace sokoban {
class GameContext;
class MultimodalInterface;
class ActionResult;
}

namespace sokoban {

class SessionContext
{
public:
    virtual GameContext&         gameContext()         = 0;
    virtual MultimodalInterface& multimodalInterface() = 0;

    virtual void         loadLevel(const std::filesystem::path& path)            = 0;
    virtual ActionResult executeCommand(const std::shared_ptr<Command>& command) = 0;
    virtual void         representAction(const ActionResult& action)             = 0;

private:
};

} // namespace sokoban