#pragma once

#include "../sokoban_core/game_object_factory.hpp"
#include "../json_utils/json_utils.hpp"
#include "objects_to_symbol_hasher.hpp"


#include <unordered_map>
#include <filesystem>
#include <iostream>

namespace sokoban {
namespace sbg {

class BaseGameObjectFactory : public GameObjectFactory
{
public: // types
    using ObjectsMap = std::unordered_map<char, std::vector<GameObjectIdSptr>>;
    using SymbolsMap = std::unordered_map<std::vector<GameObjectIdSptr>, char, ObjectsToSymbolHasher>;

public:
    BaseGameObjectFactory(const ObjectsMap& objectsMap);

    sokoban::GameObjectUptr     create(const GameObjectId& id) const override;
    std::vector<GameObjectSptr> create(char symbol) const override
    {
        const auto iter = map_.find(symbol);
        if (iter == map_.cend()) {
            throw std::runtime_error("Could't find symbol " + symbol);
        } else {
            std::vector<GameObjectSptr> objects;
            for (const auto& id : iter->second) {
                objects.push_back(std::make_shared<GameObject>(id));
            }
            return objects;
        }
    }

    char symbol(const std::vector<GameObjectSptr>& objects)
    {
        std::vector<GameObjectIdSptr> ids;
        for (const auto& object : objects) {
            //std::cout << object->id()->id() << std::endl;
            ids.push_back(object->id());
            
        }

        auto iter = symbolsMap_.find(ids);
        if (iter == symbolsMap_.cend()) {
            return 0;
        } else {
            return iter->second;
        }
    }

private:
    ObjectsMap map_;
    SymbolsMap symbolsMap_;
};

BaseGameObjectFactory::ObjectsMap loadFromJsonFile(const std::filesystem::path& path);

} // namespace sbg
} // namespace sokoban