#pragma once

#include "game_object.hpp"

#include <vector>
#include <memory>

namespace sokoban {

class Tile
{
private: // types
    using ObjectsContainer = std::vector<GameObjectSptr>;

public:
    using Iterator = ObjectsContainer::iterator;

    Iterator begin() { return objects_.begin(); }
    Iterator end() { return objects_.end(); }

private: // data
    std::vector<GameObjectSptr> objects_;
};

using TileUptr = std::unique_ptr<Tile>;

} // namespace sc