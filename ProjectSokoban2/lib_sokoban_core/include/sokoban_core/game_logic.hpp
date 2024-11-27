#pragma once

#include "direction.hpp"
#include "game_object.hpp"
#include "level_map.hpp"

namespace sokoban {

class GameLogic
{
public:

};

class BaseGameLogic
{
public:
    /*bool moveObject(Object& object, LevelMap& map);
    {
        if(!canMoveObject(...)){
          return false;
        }

        auto object = levelMap->takeObject(coordinates, object);
        levelMap->addObject(coordinates, object);
        context_->sfxService->playSound(moveSound);

        return actionResult;
    }
    */

    private:
        // GameContext context_;
};

} // namespace sokoban