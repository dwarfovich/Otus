#pragma once

#include "sokoban_core/key_definitions.hpp"
#include "base_game_action.hpp"
#include "sokoban_core/command.hpp"

#include <memory>

namespace sokoban {
namespace sbg {

class BaseActionFactory
{
public:
    std::unique_ptr<BaseGameAction> create(const Command& command) { return std::make_unique<BaseGameAction>(command.key()); }
};

} // namespace sbg
} // namespace sokoban