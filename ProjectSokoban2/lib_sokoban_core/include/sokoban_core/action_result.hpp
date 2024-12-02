#pragma once

#include "game_state.hpp"

#include <any>
#include <vector>
#include <optional>

namespace sokoban{

    class ActionResult{
public:
    bool hasNewState() const{
        return newState_.has_value();
    }

    GameState newState() const{
        return newState_.value();
    }

    private:
        std::optional<GameState> newState_;
        std::string id_;
        std::vector<std::any> data_;
};

}