#pragma once

#include "../sokoban_core/session_context.hpp"
#include "../sokoban_base_game/base_game_object_factory.hpp"
#include "../sokoban_core/command.hpp"
#include "../sokoban_core/action_result.hpp"
#include "sokoban_core/default_paths.hpp"
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
        auto objects         = loadFromJsonFile(sokoban::default_paths::addonsFolder / "Core/object_ids.json");
        gameObjectFactory_   = std::make_unique<BaseGameObjectFactory>(std::move(objects));
        game_                = std::make_unique<BaseGame>(
            loadLevelMap(default_paths::addonsFolder / "Core/level1.lm", *gameObjectFactory_));
    }

    MultimodalInterface& multimodalInterface() override { return *multimodalInterface_; }

    void         loadLevel(const std::filesystem::path& path) override { int t = 433; }
    ActionResult executeCommand(const std::shared_ptr<Command>& command) override { 
        auto action = BaseActionFactory().create(*command);
        action->perform(*this);
        // auto action = createAction(command);
        // auto result = action->execute(game_);
        // log(action);
        // action->representChanges(multimodalInterface_ );
        return {};
    }
    void         representAction(const ActionResult& action) override {}
    const LevelMap& level() const{
        return game_->map();
    } 
    void drawLevel(const LevelMap& level) override {
        system("cls");

        for(const auto& row : level.map()){
            for(const auto& tile : row){
                auto symbol  = gameObjectFactory_->symbol(tile.objects());
                if(symbol){
                    std::cout << symbol;
                } else{
                    std::cout << ' ';
                }
            }
            std::cout << '\n';
        }
    }

    Game& game() {return *game_;}

private:
    std::unique_ptr<BaseGame>                game_                = nullptr;
    std::unique_ptr<BaseMultimodalInterface> multimodalInterface_ = nullptr;
    std::unique_ptr<BaseGameObjectFactory>   gameObjectFactory_   = nullptr;
};

} // namespace sbg
} // namespace sokoban