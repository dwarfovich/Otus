#pragma once

#include "tile.hpp"
#include "coords.hpp"
#include "game_object_factory.hpp"
#include "direction.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <functional>

namespace sokoban {

class LevelMap
{
public: // types
    using Row     = std::vector<Tile>;
    using TileMap = std::vector<Row>;

public:
    LevelMap(TileMap map) : map_ { map }
    {
        for (std::size_t i = 0; i < map_.size(); ++i) {
            for (std::size_t j = 0; j < map_.size(); ++j) {
                const auto& tile = map_[i][j];
                for (const auto& object : tile){
                    if (object->id()->id() == "player"){
                        playerCoords_ = {j, i};
                        return;
                    }
                }

            }
        }
    }

    const TileMap& map() const { return map_; }
    std::optional<std::reference_wrapper<const Tile>> adjacentTile(const Coords& coords, Direction direction)const{

        const auto& row = map_[coords.y()];
        if(row.size() >= coords.x()){
            return {};
        }

        return row[coords.x() + 1];
    }

    const Tile& tile(const Coords& coords) const{
        return map_[coords.y()][coords.x()];
    }

     Tile& tile(const Coords& coords) { return map_[coords.y()][coords.x()]; }

    const Coords& playerCoords() const{
        return playerCoords_;
    }

private:
    TileMap map_;
    Coords  playerCoords_;
};

inline LevelMap::TileMap loadLevelMap(const std::filesystem::path& path, const GameObjectFactory& objectFactory)
{
    auto file = std::ifstream { path };
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open path " + path.string());
    }

    LevelMap::TileMap map;

    std::string line;
    while (std::getline(file, line)) {
        LevelMap::Row row(line.size());
        for (std::size_t i = 0; i < line.size(); ++i) {
            row[i].setObjects(objectFactory.create(line[i]));
        }
        map.push_back(std::move(row));
    }

    return map;
}

} // namespace sokoban