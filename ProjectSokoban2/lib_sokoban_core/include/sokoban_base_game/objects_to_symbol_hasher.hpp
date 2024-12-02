#pragma once

namespace sokoban {
namespace sbg {

struct ObjectsToSymbolHasher
{
    std::size_t operator()(const std::vector<GameObjectIdSptr>& objects) const
    {
        std::size_t hash = 0;
        for (const auto& object : objects) {
            hash += std::hash<std::string>()(object->id());
        }

        return hash;
    }
};

struct ObjectsToSymbolComparator
{
    std::size_t operator()(const std::vector<GameObjectIdSptr>& lhs, const std::vector<GameObjectIdSptr>& rhs) const
    {
        if(lhs.size() != rhs.size()){
            return false;
        }

        for(const auto& left : lhs){
            auto iter = std::find_if(rhs.cbegin(), rhs.cend(),[&left](const auto& rhs){return left->id() == rhs->id();});
            if(iter == rhs.cend()){
                return false;
            }
        }

        return true;
    }
};

} // namespace sbg
} // namespace sokoban