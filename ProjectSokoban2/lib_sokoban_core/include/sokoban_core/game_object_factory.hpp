#pragma once

#include "game_object_id.hpp"
#include "game_object.hpp"

namespace sokoban {

class GameObjectFactory
{
public:
    virtual ~GameObjectFactory() = default;

    virtual GameObjectUptr create(const GameObjectId& id) const = 0;

private:
};

} // namespace sokoban