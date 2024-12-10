#pragma once

#include "sokoban_core/session_context.hpp"
#include "tui/console.hpp"
#include "base_game_object_factory.hpp"
#include "sokoban_core/command.hpp"
#include "sokoban_core/action_result.hpp"
#include "sokoban_core/default_paths.hpp"
#include "sokoban_core/action_logger.hpp"
#include "base_game.hpp"
#include "base_multimodal_interface.hpp"
#include "base_action_factory.hpp"

#include <filesystem>
#include <memory>
#include <iostream>

namespace sokoban {
namespace sbg {

class BaseSessionContext : public SessionContext
{
public:
    BaseSessionContext() { multimodalInterface_ = std::make_unique<BaseMultimodalInterface>(); }

    MultimodalInterface&  multimodalInterface() override { return *multimodalInterface_; }
    Game&                 game() override { return *game_; }
    std::pair<bool, bool> executeCommand(const std::shared_ptr<Command>& command) override
    {
        auto action                  = actionFactory_.create(*command);
        auto [success, gameFinished] = action->perform(*game_);
        if (success) {
            redrawGame();
        }
        return { success, gameFinished };
    }

    std::filesystem::path generateLogFilePath(const std::string& levelName) const
    {
        std::filesystem::path path { default_paths::modsFolder / "Core/Logs/" };
        std::time_t           t = std::time(nullptr);
        char                  mbstr[100];

        path += levelName;
        if (std::strftime(mbstr, sizeof(mbstr), "-%Y-%m-%d_%H_%M_%S", std::localtime(&t))) {
            path += mbstr;
        }
        path += ".gl";

        return path;
    }

    void startGame() override
    {
        auto path = std::filesystem::current_path() / sokoban::default_paths::modsFolder / "BaseGame/object_ids.json";
        auto objects       = loadFromJsonFile(path);
        gameObjectFactory_ = std::make_unique<BaseGameObjectFactory>(std::move(objects));
        const std::filesystem::path levelpath =
            std::filesystem::current_path() / sokoban::default_paths::modsFolder / "BaseGame/level1.lm";
        auto [map, playerCoords] = loadLevelMap(levelpath, *gameObjectFactory_);
        game_                    = std::make_unique<BaseGame>(std::move(map), std::move(playerCoords));
        console().clear();
        drawLevel(std::cout, game_->map());
    }
    void redrawGame()
    {
        console().clear();
        drawLevel(std::cout, game_->map());
    }

    bool supportsSaveGames() const override { return true; }
    void saveGame(const std::filesystem::path& path) const override
    {
        std::ofstream stream { path };
        if (!stream.is_open()) {
            throw std::runtime_error("Failed to open file for saving game - " + path.string());
        }

        drawLevel(stream, game_->map());
    }
    virtual void loadGame(const std::filesystem::path& path) override {}

    void drawLevel(std::ostream& stream, const RectangleTileMap& map) const
    {
        for (const auto& row : game_->map()) {
            for (const auto& tile : row) {
                auto symbol = gameObjectFactory_->symbol(tile.objects());
                if (symbol) {
                    stream << symbol;
                } else {
                    stream << ' ';
                }
            }
            stream << '\n';
        }
    }

private:
    std::unique_ptr<BaseGame>                game_                = nullptr;
    std::unique_ptr<BaseMultimodalInterface> multimodalInterface_ = nullptr;
    std::unique_ptr<BaseGameObjectFactory>   gameObjectFactory_   = nullptr;
    std::unique_ptr<ActionLogger>            actionLogger_        = nullptr;
    BaseActionFactory                        actionFactory_;
};

} // namespace sbg
} // namespace sokoban