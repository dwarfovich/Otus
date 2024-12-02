#include "base_game_object_factory.hpp"

#include <iostream>

namespace sokoban {
namespace sbg {

BaseGameObjectFactory::BaseGameObjectFactory(const ObjectsMap& objectsMap) : map_ { objectsMap }
{
    for (const auto& iter : map_) {
        symbolsMap_[iter.second] = iter.first;
    }
}

GameObjectUptr BaseGameObjectFactory::create(const GameObjectId& id) const
{
    return nullptr;
}

BaseGameObjectFactory::ObjectsMap loadFromJsonFile(const std::filesystem::path& path)
{
    BaseGameObjectFactory::ObjectsMap map;
    auto                              json = json_utils::loadFromFile(path);
    for (auto iter = json.begin(); iter != json.end(); ++iter) {
        if (iter.value().is_string()) {
            if (iter.value() != "empty") {
                map[iter.key().front()].push_back(std::make_shared<GameObjectId>(iter.value()));
            }
        } else if (iter.value().is_array()) {
            auto idIter = map.insert({ iter.key().front(), {} });
            for (const auto& id : iter.value()) {
                if (id != "empty") {
                    idIter.first->second.push_back(std::make_shared<GameObjectId>(id));
                }
            }
        }
    }

    return map;
}

} // namespace sbg
} // namespace sokoban