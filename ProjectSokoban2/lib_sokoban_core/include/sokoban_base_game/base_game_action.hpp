#pragma once

#include "sokoban_core/game_action.hpp"
#include "tui/key_definitions.hpp"

namespace sokoban {
namespace sbg {

class BaseSessionContext;

class BaseGameAction : public GameAction
{
public:
    BaseGameAction(sokoban::tui::Key key) : key_ { key } {}
    bool perform(BaseSessionContext& context);

private:
    sokoban::tui::Key key_;
};

} // namespace sbg
} // namespace sokoban