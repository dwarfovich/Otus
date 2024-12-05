#pragma once

#include "sokoban_core/session_context.hpp"
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
    BaseSessionContext()
    {
        multimodalInterface_ = std::make_unique<BaseMultimodalInterface>();
        //auto objects         = loadFromJsonFile("object_ids.json");
        //gameObjectFactory_   = std::make_unique<BaseGameObjectFactory>(std::move(objects));
        //game_                = std::make_unique<BaseGame>(
        //    loadLevelMap(default_paths::addonsFolder / "Core/level1.lm", *gameObjectFactory_));*/
        //actionLogger_ = std::make_unique<ActionLogger>(generateLogFilepath("level1"));
    }

    MultimodalInterface& multimodalInterface() override { return *multimodalInterface_; }

    bool executeCommand(const std::shared_ptr<Command>& command) override
    {
        auto action = BaseActionFactory().create(*command);
        auto [success, gameFinished] = action->perform(*this);
        if(success && actionLogger_){
            actionLogger_->log(action->key());
        }
        return gameFinished;
    }
    //void drawLevel(const BaseGame::RectangleTileMap& level)
    //{
    //    system("cls");

    //    for (const auto& row : game_->map()) {
    //        for (const auto& tile : row) {
    //            auto symbol = gameObjectFactory_->symbol(tile.objects());
    //            if (symbol) {
    //                std::cout << symbol;
    //            } else {
    //                std::cout << ' ';
    //            }
    //        }
    //        std::cout << '\n';
    //    }
    //}

    //BaseGame& game() { return *game_; }

    std::filesystem::path generateLogFilepath(const std::string& levelName) const
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

    void startGame() override{
                // auto objects         = loadFromJsonFile("object_ids.json");
        // gameObjectFactory_   = std::make_unique<BaseGameObjectFactory>(std::move(objects));
        // game_                = std::make_unique<BaseGame>(
        //     loadLevelMap(default_paths::addonsFolder / "Core/level1.lm", *gameObjectFactory_));*/
        // actionLogger_ = std::make_unique<ActionLogger>(generateLogFilepath("level1"));

        //game_ = std::make_unique<BaseGame>(std::filesystem::current_path() / sokoban::default_paths::modsFolder/ "level1.lm");
    }

private:
    std::unique_ptr<BaseGame>                game_                = nullptr;
    std::unique_ptr<BaseMultimodalInterface> multimodalInterface_ = nullptr;
    std::unique_ptr<BaseGameObjectFactory>   gameObjectFactory_   = nullptr;
    std::unique_ptr<ActionLogger>            actionLogger_        = nullptr;
};

} // namespace sbg
} // namespace sokoban