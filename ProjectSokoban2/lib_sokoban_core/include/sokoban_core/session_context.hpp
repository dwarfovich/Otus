#pragma once

// #include "action_result.hpp"
#include "command.hpp"
#include "tui/console.hpp"

#include <memory>
#include <filesystem>

namespace sokoban {
class MultimodalInterface;
} // namespace sokoban

namespace sokoban {

class SessionContext
{
public:
    virtual MultimodalInterface& multimodalInterface()                                   = 0;
    virtual bool                 executeCommand(const std::shared_ptr<Command>& command) = 0;
    virtual void startGame() = 0;

    void setConsole(const std::shared_ptr<tui::Console>& console){
        console_ = console;
    }
    tui::Console& console() {
        return *console_;
    }

    protected:
        std::shared_ptr<tui::Console> console_;
};

} // namespace sokoban