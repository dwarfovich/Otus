#pragma once

#include "../sokoban_core/session_context.hpp"
#include "../sokoban_base_game/base_game_object_factory.hpp"
#include "../sokoban_core/command.hpp"
#include "../sokoban_core/action_result.hpp"
#include "base_game.hpp"
#include "base_multimodal_interface.hpp"
#include "base_game_context.hpp"

#include <filesystem>
#include <memory>

namespace sokoban {
namespace sbg {

class BaseSessionContext : public SessionContext
{
public:
    BaseSessionContext(){
        multimodalInterface_ = std::make_unique<BaseMultimodalInterface>();
        gameContext_ = std::make_unique<BaseGameContext>();
        auto objects         = loadFromJsonFile(default_paths::addonsFolder / "Core/object_ids.json");
        gameObjectFactory_   = std::make_unique<BaseGameObjectFactory>(std::move(objects));
        game_                = std::make_unique<BaseGame>(
            loadLevelMap(default_paths::addonsFolder / "Core/level1.lm", *gameObjectFactory_));
    }

    GameContext&         gameContext() override { return *gameContext_; };
    MultimodalInterface& multimodalInterface() override { return *multimodalInterface_; }

    void         loadLevel(const std::filesystem::path& path) override {
        int t =433;
    }
    ActionResult executeCommand(const std::shared_ptr<Command>& command) override { return {}; }
    void         representAction(const ActionResult& action) override {}

private:
    std::unique_ptr<BaseGame>                game_                = nullptr;
    std::unique_ptr<BaseMultimodalInterface> multimodalInterface_ = nullptr;
    std::unique_ptr<BaseGameContext>         gameContext_         = nullptr;
    std::unique_ptr<BaseGameObjectFactory>         gameObjectFactory_         = nullptr;
};

} // namespace sbg
} // namespace sokoban