#pragma once

#include "tile.hpp"
#include "coords.hpp"
#include "direction.hpp"
#include "game_object_factory.hpp"
#include "json_utils/json_utils.hpp"

#include <optional>
#include <limits>
#include <istream>

namespace sokoban {

class RectangleTileMap
{
public: // types
    using Row     = std::vector<Tile>;
    using ConstIterator = std::vector<Row>::const_iterator;

public: // methods
    RectangleTileMap()=default;
    RectangleTileMap(std::vector<Row> map): map_{map}{}

    ConstIterator begin() const {return map_.cbegin();};
    ConstIterator end() const { return map_.cend(); };

    bool loadMapFromJson(std::istream& stream, const GameObjectFactory& objectFactory)
    {
        map_.clear();

        std::string line;
        while (std::getline(stream, line)) {
            RectangleTileMap::Row row(line.size());
            for (std::size_t i = 0; i < line.size(); ++i) {
                row[i].setObjects(objectFactory.create(line[i]));

            }
            map_.push_back(std::move(row));
        }

        return false;
    }
    

    const std::vector<Row>& map() const{
        return map_;
    }

    std::size_t size() const {
        return map_.size();
    }

    std::optional<std::reference_wrapper<const Tile>> adjacentTile(const Coords& coords, Direction direction) const
    {
        if (coords.y() >= map_.size()) {
            return {};
        }

        const auto& row = map_[coords.y()];
        if (direction == Direction::Up) {
            if (coords.y() == 0) {
                return {};
            }
            return map_[coords.y() - 1][coords.x()];
        }
        if (direction == Direction::Right) {
            if (row.size() <= coords.x() - 1) {
                return {};
            }
            return row[coords.x() + 1];
        }
        if (direction == Direction::Down) {
            if (coords.y() >= map_.size() - 1) {
                return {};
            }
            return map_[coords.y() + 1][coords.x()];
        }
        if (direction == Direction::Left) {
            if (coords.x() == 0) {
                return {};
            }
            return row[coords.x() - 1];
        }

        return {};
    }

    std::optional<Coords> adjacentCoords(const Coords& coords, Direction direction) const
    {
        if (direction == Direction::Left) {
            if (coords.x() == 0){
                return {};
            }
            return { {coords.x() - 1, coords.y()} };
        }
        if (direction == Direction::Right) {
            if(coords.x() == std::numeric_limits<decltype(coords.x())>::max()){
                return {};
            }
            return {{ coords.x() + 1, coords.y() }};
        }
        if (direction == Direction::Up) {
            if (coords.y() == 0) {
                return {};
            }
            return {{ coords.x(), coords.y() - 1 }};
        }
        if (direction == Direction::Down) {
            if (coords.y() == std::numeric_limits<decltype(coords.y())>::max()) {
                return {};
            }
            return {{ coords.x(), coords.y() + 1 }};
        }

        return {};
    }

     void moveObject(std::shared_ptr<GameObject> object, const Coords& sourceCoords, const Coords& targetCoords)
    {
         auto& tilet = tile(sourceCoords);
         tilet.removeObject(object);
         tile(targetCoords).addObject(object);
    }

     Tile& tile(const Coords& coords){
         return map_[coords.y()][coords.x()];
     }

private: // data
    std::vector<Row> map_;
    Coords playerCoords_;
};

} // namespace sokoban