#pragma once

#include "game_object_id.hpp"

#include <memory>
#include <vector>

namespace sokoban {

class GameObject
{
public:
    GameObject(const std::shared_ptr<GameObjectId>& id) : id_{id}{}

    const std::shared_ptr<GameObjectId>& id() const{
        return id_;
    }

private:
    std::shared_ptr<GameObjectId>              id_ = nullptr;
    std::vector<std::shared_ptr<GameObjectId>> childObjects_;
};

using GameObjectSptr = std::shared_ptr<GameObject>;
using GameObjectUptr = std::unique_ptr<GameObject>;

} // namespace sokoban