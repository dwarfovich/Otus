#pragma once

#include "game_object_id.hpp"

#include <memory>
#include <vector>

namespace sokoban {

class GameObject{
public:

    private:
        std::shared_ptr<GameObjectId> id_ = nullptr; 
        std::vector<std::shared_ptr<GameObjectId>> childObjects_;
};
  
using GameObjectSptr = std::shared_ptr<GameObject>;
using GameObjectUptr = std::unique_ptr<GameObject>;

}