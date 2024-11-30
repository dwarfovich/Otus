#pragma once

#include "game_object_id.hpp"
#include "game_object.hpp"

#include <unordered_map>
#include <vector>

namespace sokoban {

class GameObjectFactory
{
public:
    
    virtual ~GameObjectFactory() = default;

    /*void setObjectsMap(const std::unordered_map<char, std::vector<GameObject>>& map){
    }*/

    virtual GameObjectUptr create(const GameObjectId& id) const = 0;
    virtual std::vector<GameObjectSptr> create(char symbol) const            = 0;

private:
};

} // namespace sokoban