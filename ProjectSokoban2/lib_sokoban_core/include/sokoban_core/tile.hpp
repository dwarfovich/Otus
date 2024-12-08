#pragma once

#include "game_object.hpp"

#include <vector>
#include <memory>

namespace sokoban {

class Tile
{
private: // types
    using ObjectsContainer = std::vector<GameObjectSptr>;

public: // types
    using Iterator      = ObjectsContainer::iterator;
    using ConstIterator = ObjectsContainer::const_iterator;

public: // methods
    Tile() = default;
    Tile(std::vector<GameObjectSptr> objects) : objects_ { objects } {}

    Iterator      begin() { return objects_.begin(); }
    Iterator      end() { return objects_.end(); }
    ConstIterator begin() const { return objects_.cbegin(); }
    ConstIterator end() const { return objects_.cend(); }

    void setObjects(std::vector<GameObjectSptr>&& objects) { objects_ = std::move(objects); }
    const std::vector<GameObjectSptr>& objects() const { return objects_; }
        bool removeObject(const GameObjectSptr& object)
    {
        objects_.erase(find(objects_.begin(), objects_.end(), object));
        return true;
    }
    void addObject(const GameObjectSptr& object) { objects_.push_back(object); }

private: // data
    std::vector<GameObjectSptr> objects_;
};

using TileUptr = std::unique_ptr<Tile>;

} // namespace sokoban