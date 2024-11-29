#pragma once

//#include "session_context.hpp"
#include "../sokoban_core/session_context.hpp"
//#include "game_context.hpp"
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
        game_ = std::make_unique<BaseGame>();
        multimodalInterface_ = std::make_unique<BaseMultimodalInterface>();
        gameContext_ = std::make_unique<BaseGameContext>();
    }
    GameContext&         gameContext() override { return *gameContext_; };
    MultimodalInterface& multimodalInterface() override { return (MultimodalInterface&)*multimodalInterface_; }

    void         loadLevel(const std::filesystem::path& path) override {
        int t =433;
    }
    ActionResult executeCommand(const std::shared_ptr<Command>& command) override { return {}; }
    void         representAction(const ActionResult& action) override {}

private:
    std::unique_ptr<BaseGame>                game_                = nullptr;
    std::unique_ptr<BaseMultimodalInterface> multimodalInterface_ = nullptr;
    std::unique_ptr<BaseGameContext>         gameContext_         = nullptr;
};

} // namespace sbg
} // namespace sokoban