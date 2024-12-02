#pragma once

#include "sokoban_core/game.hpp"
#include "sokoban_core/game_action.hpp"

namespace sokoban {
namespace sbg {

class BaseGame : public Game
{
public:
    BaseGame(LevelMap::TileMap map) : Game { map } {}

    bool isFinished() const
    {
        for (const auto& row : map_.map()) {
            for (const auto& tile : row) {
                auto targetIter = std::find_if(tile.begin(), tile.begin(), [](const auto& object) {
                    return object->id()->id() == "target";
                });
                if(targetIter != tile.end()){
                    auto crateIter = std::find_if(tile.begin(), tile.begin(), [](const auto& object) {
                        return object->id()->id() == "crate";
                    });
                    if(crateIter == tile.end()){
                        return false;
                    }
                }
            }
        }

        return true;
    }

private:
};

} // namespace sbg
} // namespace sokoban