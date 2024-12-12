#pragma once

#include "command.hpp"
#include "game_state.hpp"
#include "tui/console.hpp"

#include <memory>
#include <filesystem>

namespace sokoban {
class MultimodalInterface;
class Game;
} // namespace sokoban

namespace sokoban {

class SessionContext
{
public:
    virtual MultimodalInterface&  multimodalInterface()                                   = 0;
    virtual std::pair<bool, GameState> executeCommand(const std::shared_ptr<Command>& command) = 0;
    virtual Game&                 game()                                                  = 0;
    virtual void                  startGame()                                             = 0;
    virtual void                  loadNextLevel()                                      = 0;
    virtual void                  incrementLevelNumber() {}
    virtual bool                  hasNextLevel() const { return false; }
    virtual bool                  supportsSaveGames() const { return false; }
    virtual void                  saveGame(const std::filesystem::path& path) const {}
    virtual void                  loadGame(const std::filesystem::path& path) {}

    void          setConsole(const std::shared_ptr<tui::Console>& console) { console_ = console; }
    tui::Console& console() { return *console_; }
    void          setModFolderPath(const std::filesystem::path& path) { modFolderPath_ = path; }

protected:
    std::shared_ptr<tui::Console> console_;
    std::filesystem::path         modFolderPath_;
};

} // namespace sokoban