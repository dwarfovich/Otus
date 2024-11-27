#pragma once

#include "game_object_id.h"

#include <memory>

namespace sokoban {

class GameObject{
public:

    private:
        std::shared_ptr<GameObjectId> id_ = nullptr;
};
  
using GameObjectSptr = std::shared_ptr<GameObject>;
using GameObjectUptr = std::unique_ptr<GameObject>;

}