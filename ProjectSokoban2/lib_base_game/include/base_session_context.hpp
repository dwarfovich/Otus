#pragma once

#include "base_game.hpp"
#include "base_multimodal_interface.hpp"
#include "base_game_object_factory.hpp"
#include "base_action_factory.hpp"
#include "sokoban_core/session_context.hpp"
#include "sokoban_core/command.hpp"
#include "sokoban_core/action_result.hpp"
#include "sokoban_core/default_paths.hpp"
#include "sokoban_core/action_logger.hpp"
#include "sokoban_core/campaign.hpp"
#include "tui/console.hpp"

#include <filesystem>
#include <memory>
#include <iostream>

namespace sokoban {
namespace sbg {

class BaseSessionContext : public SessionContext
{
public:
    BaseSessionContext() { multimodalInterface_ = std::make_unique<BaseMultimodalInterface>(); }

    MultimodalInterface&       multimodalInterface() override { return *multimodalInterface_; }
    std::pair<bool, GameState> executeCommand(const std::shared_ptr<Command>& command) override
    {
        auto action                  = actionFactory_.create(*command);
        auto [success, gameFinished] = action->perform(*game_);
        if (success) {
            drawLevel();
        }
        return { success, gameFinished };
    }
    void drawLevel() override
    {
        console_->clear();
        drawLevel(std::cout);
    }

    void initialize() override
    {
        auto campaignPath =
            std::filesystem::current_path() / sokoban::default_paths::modsFolder / "BaseGame/campaign.json";
        auto campaignJson = json_utils::loadFromFile(campaignPath);
        campaign_         = std::make_unique<Campaign>();
        campaign_->load(campaignJson);
        auto path = std::filesystem::current_path() / sokoban::default_paths::modsFolder / "BaseGame/object_ids.json";
        auto objects       = loadFromJsonFile(path);
        gameObjectFactory_ = std::make_unique<BaseGameObjectFactory>(std::move(objects));
    }

    void loadLevel(const std::filesystem::path& path = {}) override
    {
        const std::filesystem::path levelpath = path.empty() ? std::filesystem::current_path()
                                                                   / sokoban::default_paths::modsFolder / "BaseGame/"
                                                                   / campaign_->currentLevelName()
                                                             : path;
        currentlevelPath_        = sokoban::default_paths::modsFolder / "BaseGame/" / campaign_->currentLevelName();
        auto [map, playerCoords] = loadLevelMap(levelpath, *gameObjectFactory_);
        if (!game_) {
            game_ = std::make_unique<BaseGame>();
        }
        game_->setMap(std::move(map));
        game_->setPlayerCoords(std::move(playerCoords));
    }
    void incrementLevelNumber() override { campaign_->setCurrentLevel(campaign_->currentLevel() + 1); }
    bool supportsSaveGames() const override { return true; }
    bool hasNextLevel() const override { return campaign_->hasNextLevel(); }
    void saveGame(std::ostream& stream) const
    {
        stream << campaign_->currentLevel() << '\n';
        drawLevel(stream);
    }

    void loadGame(std::istream& stream) override
    {
        auto objectsPath =
            std::filesystem::current_path() / sokoban::default_paths::modsFolder / "BaseGame/object_ids.json";
        auto objects       = loadFromJsonFile(objectsPath);
        gameObjectFactory_ = std::make_unique<BaseGameObjectFactory>(std::move(objects));
        if (!campaign_) {
            campaign_->load(std::filesystem::current_path() / sokoban::default_paths::modsFolder
                            / "BaseGame/campaign.json");
        }

        auto [map, playerCoords, level] = loadSaveGame(stream, *gameObjectFactory_);
        campaign_->setCurrentLevel(level);
        game_ = std::make_unique<BaseGame>(std::move(map), std::move(playerCoords));
    }

private: // methods
    void drawLevel(std::ostream& stream) const
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

    std::string achievement() const override { return "Base campaign completed!"; }

private:
    std::unique_ptr<BaseGame>                game_                = nullptr;
    std::unique_ptr<BaseMultimodalInterface> multimodalInterface_ = nullptr;
    std::unique_ptr<BaseGameObjectFactory>   gameObjectFactory_   = nullptr;
    std::unique_ptr<ActionLogger>            actionLogger_        = nullptr;
    std::unique_ptr<Campaign>                campaign_            = nullptr;
    BaseActionFactory                        actionFactory_;
};

} // namespace sbg
} // namespace sokoban