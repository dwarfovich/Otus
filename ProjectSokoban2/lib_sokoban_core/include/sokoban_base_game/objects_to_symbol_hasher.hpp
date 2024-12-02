#pragma once

namespace sokoban{
    namespace sbg{
    
        struct ObjectsToSymbolHasher
    {
            std::size_t operator()(const std::vector<GameObjectIdSptr>& objects) const{
                std::size_t hash = 0;
                for(const auto& object: objects){
                    hash += std::hash<std::string>()(object->id());
                }

                return hash;
            }
        };

    }
}