#pragma once

#include "sokoban_core/game.hpp"
#include "sokoban_core/game_state.hpp"
#include "sokoban_core/game_action.hpp"
#include "sokoban_core/tile.hpp"
#include "sokoban_core/tile_map.hpp"
#include "sokoban_core/coords.hpp"
#include "sokoban_core/direction.hpp"

class ModGame : public sokoban::Game
{
public:
    ModGame() = default;
    ModGame(sokoban::RectangleTileMap map, sokoban::Coords playerCoords) : map_ { map }, playerCoords_ { playerCoords }
    {
        /*for (std::size_t i = 0; i < map_.size(); ++i) {
            for (std::size_t j = 0; j < map_.size(); ++j) {
                const auto& tile = map_[i][j];
                for (const auto& object : tile) {
                    if (object->id()->id() == "player") {
                        playerCoords_ = { j, i };
                        return;
                    }
                }
            }
        }*/
    }

    bool gameState() const
    {
        /*for (const auto& row : map_) {
            for (const auto& tile : row) {
                auto targetIter = std::find_if(tile.begin(), tile.end(), [](const auto& object) {
                    return object->id()->id() == "target";
                });
                if (targetIter != tile.end()) {
                    auto crateIter = std::find_if(tile.begin(), tile.end(), [](const auto& object) {
                        return object->id()->id() == "crate";
                    });
                    if (crateIter == tile.end()) {
                        return false;
                    }
                }
            }
        }*/

        return true;
    }
    void setMap(const sokoban::RectangleTileMap& map) { map_ = map; }

    const sokoban::RectangleTileMap& map() const { return map_; }

    sokoban::RectangleTileMap& map() { return map_; }

    const sokoban::Tile& tile(const sokoban::Coords& coords) const { return map_.map()[coords.y()][coords.x()]; }

    //Tile& tile(const Coords& coords) { return map_.map()[coords.y()][coords.x()]; }

    const sokoban::Coords& playerCoords() const { return playerCoords_; }

    void setPlayerCoords(const sokoban::Coords& coords) { playerCoords_ = coords; }
    void moveObject(std::shared_ptr<sokoban::GameObject> object,
                    const sokoban::Coords&               sourceCoords,
                    const sokoban::Coords&               targetCoords)
    {
        /*auto& tilet = tile(sourceCoords);
        tilet.removeObject(object);
        tile(targetCoords).addObject(object);*/
    }

private:
    sokoban::RectangleTileMap map_;
    sokoban::Coords           playerCoords_;
    sokoban::GameState        gameState_ = sokoban::GameState::InProgress;
};

//inline BaseGame::TileMap loadLevelMap(const std::filesystem::path& path, const GameObjectFactory& objectFactory)
//{
//    auto file = std::ifstream { path };
//    if (!file.is_open()) {
//        throw std::runtime_error("Failed to open path " + path.string());
//    }
//
//    BaseGame::TileMap map;
//
//    std::string line;
//    while (std::getline(file, line)) {
//        BaseGame::Row row(line.size());
//        for (std::size_t i = 0; i < line.size(); ++i) {
//            row[i].setObjects(objectFactory.create(line[i]));
//        }
//        map.push_back(std::move(row));
//    }
//
//    return map;
//}

