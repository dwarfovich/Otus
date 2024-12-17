#pragma once

#include "command.hpp"
#include "game_state.hpp"
#include "player.hpp"
#include "tui/console.hpp"

#include <memory>
#include <filesystem>
#include <iosfwd>

namespace sokoban {
class MultimodalInterface;
class Game;
} // namespace sokoban

namespace sokoban {

class SessionContext
{
public:
    virtual MultimodalInterface&       multimodalInterface()                                   = 0;
    virtual void                       initialize()                                            = 0;
    virtual std::pair<bool, GameState> executeCommand(const std::shared_ptr<Command>& command) = 0;
    virtual void                       drawLevel()                                             = 0;
    virtual void                       loadLevel(const std::filesystem::path& path = {}) = 0;

    virtual void        incrementLevelNumber() {}
    virtual bool        hasNextLevel() const { return false; }
    virtual bool        supportsSaveGames() const { return false; }
    virtual void        saveGame(std::ostream& stream) const {}
    virtual void        loadGame(std::istream& stream) {}
    virtual std::string achievement() const { return {}; }

    void                           setPlayer(const std::shared_ptr<Player>& player) { player_ = player; }
    const std::shared_ptr<Player>& player() { return player_; }
    void                           setConsole(const std::shared_ptr<tui::Console>& console) { console_ = console; }
    tui::Console&                  console() { return *console_; }
    void                           setModFolderPath(const std::filesystem::path& path) { modFolderPath_ = path; }
    const std::filesystem::path&   modFolderPath() const { return modFolderPath_; }
    const std::filesystem::path&   currentLevelPath() const { return currentlevelPath_; }

protected:
    std::shared_ptr<Player>       player_;
    std::shared_ptr<tui::Console> console_;
    std::filesystem::path         modFolderPath_;
    std::filesystem::path         currentlevelPath_;
};

} // namespace sokoban