#pragma once

#include "tile.hpp"
#include "coords.hpp"
#include "direction.hpp"

#include <optional>
#include <limits>

namespace sokoban {

class RectangleTileMap
{
private: // types
    using Row     = std::vector<Tile>;

public:
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

private: // data
    std::vector<Row> map_;
};

} // namespace sokoban