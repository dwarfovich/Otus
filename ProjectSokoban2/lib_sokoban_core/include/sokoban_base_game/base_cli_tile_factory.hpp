#pragma once

#include "../sokoban_core/tile_factory.hpp"
#include "../json_utils/json_utils.hpp"

#include <filesystem>
#include <unordered_map>

namespace sokoban {
namespace sbg {

class BaseCliTileFactory : public TileFactory
{
public:
    BaseCliTileFactory(const std::filesystem::path& path) { 
        initializeFromJson(path);
    }

    TileUptr create() const override { return nullptr; }

private: // methods
    void initializeFromJson(const std::filesystem::path& path){
        auto json = json_utils::loadFromFile(path);
    }

private: // data
    std::unordered_map<char, std::string> tilesMap_;
};

} // namespace sbg
} // namespace sokoban