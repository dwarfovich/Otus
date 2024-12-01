#pragma once

#include "tile_id.hpp"
#include "game_object.hpp"

#include <vector>
#include <memory>

namespace sokoban {

class Tile
{
private: // types
    using ObjectsContainer = std::vector<GameObjectSptr>;

public: // types
    using Iterator = ObjectsContainer::iterator;

    public: // methods
        Tile() = default;
    Tile(std::vector<GameObjectSptr> objects) : objects_ { objects } {}

    /*Tile(const std::shared_ptr<TileId>& id) : id_{id}{
    }*/


    Iterator begin() { return objects_.begin(); }
    Iterator end() { return objects_.end(); }

    void setObjects(std::vector<GameObjectSptr>&& objects){
        objects_ = std::move(objects);
    }

private: // data
    //std::shared_ptr<TileId> id_;
    std::vector<GameObjectSptr> objects_;
};

using TileUptr = std::unique_ptr<Tile>;

} // namespace sc