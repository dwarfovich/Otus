#pragma once

#include "tile.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace sokoban {

class LevelMap
{
public:
    LevelMap(std::vector<std::vector<Tile>> map) : map_ { map } {}

private:
    std::vector<std::vector<Tile>> map_;
};

std::vector<std::vector<Tile>> loadLevelMap(const std::filesystem::path& path, const GameObjectFactory& objectFactory)
{
    auto file = std::ifstream{path};
    if(!file.is_open()){
        throw std::runtime_error("Failed to open path " + path.string());
    }

    std::string line;
    while(std::getline(file, line)){

    }

    return {};
}

} // namespace sokoban