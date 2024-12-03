#pragma once

#include "sokoban_core/game_action.hpp"
#include "sokoban_core/key_definitions.hpp"

namespace sokoban {
namespace sbg {

class BaseSessionContext;

class BaseGameAction : public GameAction
{
public:
    BaseGameAction(Key key) : key_ { key } {}

    Key key() const{ return key_; }
    std::pair<bool, bool> perform(BaseSessionContext& context);

private:
    Key key_;
};

} // namespace sbg
} // namespace sokoban