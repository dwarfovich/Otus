#pragma once

#include "game_object_factory.hpp"

#include <unordered_map>

namespace sokoban {
namespace sbg {

class BaseGameObjectFactory : public GameObjectFactory
{
public: // types
    using ObjectsMap = std::unordered_map<char, std::vector<GameObjectIdSptr>>;

public:
    BaseGameObjectFactory(const ObjectsMap& objectsMap) : map_ { map } {}

    GameObjectSptr create(const GameObjectId& id) const override { return nullptr; }

private:
    ObjectsMap map_;
};

} // namespace sbg
} // namespace sokoban