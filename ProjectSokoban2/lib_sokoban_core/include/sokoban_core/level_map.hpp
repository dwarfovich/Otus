#pragma once

#include "tile.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace sokoban {

class LevelMap
{
public: // types
    using Row = std::vector<Tile>;
    using TileMap = std::vector<Row>;

public:
    LevelMap(TileMap map) : map_ { map } {}

private:
    TileMap map_;
};

LevelMap::TileMap loadLevelMap(const std::filesystem::path& path, const GameObjectFactory& objectFactory)
{
    auto file = std::ifstream{path};
    if(!file.is_open()){
        throw std::runtime_error("Failed to open path " + path.string());
    }

    LevelMap::TileMap map;

    std::string line;
    while(std::getline(file, line)){
        LevelMap::Row row (line.size());

    }

    return {};
}

} // namespace sokoban