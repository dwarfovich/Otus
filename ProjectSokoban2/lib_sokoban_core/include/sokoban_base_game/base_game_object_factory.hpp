#pragma once

#include "../sokoban_core/game_object_factory.hpp"
#include "../json_utils/json_utils.hpp"

#include <unordered_map>
#include <filesystem>

namespace sokoban {
namespace sbg {

class BaseGameObjectFactory : public GameObjectFactory
{
public: // types
    using ObjectsMap = std::unordered_map<char, std::vector<GameObjectIdSptr>>;

public:
    BaseGameObjectFactory(const ObjectsMap& objectsMap);

    sokoban::GameObjectUptr create(const GameObjectId& id) const override;
    std::vector<GameObjectSptr> create(char symbol) const override {
        const auto iter = map_.find(symbol);
        if(iter ==map_.cend()){
            throw std::runtime_error("Could't find symbol " + symbol);
        } else{
            std::vector<GameObjectSptr> objects;
            for (const auto& id : iter->second){
                objects.push_back(std::make_shared<GameObject>(id));
            }
            return objects;
        }
    }

private:
    ObjectsMap map_;
};

BaseGameObjectFactory::ObjectsMap loadFromJsonFile(const std::filesystem::path& path);

} // namespace sbg
} // namespace sokoban