#pragma once

#include "sokoban_core/game.hpp"
#include "sokoban_core/game_action.hpp"
#include "sokoban_core/tile.hpp"
#include "sokoban_core/tile_map.hpp"
#include "sokoban_core/coords.hpp"
#include "sokoban_core/direction.hpp"

namespace sokoban {
namespace sbg {

class BaseGame : public Game
{
public:
    BaseGame(RectangleTileMap map) : map_ { map } {
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

    bool isFinished() const
    {
        /*for (const auto& row : map_) {
            for (const auto& tile : row) {
                auto targetIter = std::find_if(tile.begin(), tile.end(), [](const auto& object) {
                    return object->id()->id() == "target";
                });
                if(targetIter != tile.end()){
                    auto crateIter = std::find_if(tile.begin(), tile.end(), [](const auto& object) {
                        return object->id()->id() == "crate";
                    });
                    if(crateIter == tile.end()){
                        return false;
                    }
                }
            }
        }*/

        return true;
    }
    void setMap(const RectangleTileMap& map) { map_ = map; }

    const RectangleTileMap& map() const { return map_; }

    RectangleTileMap& map() { return map_; }

    const Tile& tile(const Coords& coords) const { return map_.map()[coords.y()][coords.x()]; }

//    Tile& tile(const Coords& coords) { return map_.map()[coords.y()][coords.x()]; }

    const Coords& playerCoords() const { return playerCoords_; }

    void setPlayerCoords(const Coords& coords) { playerCoords_ = coords; }
    void moveObject(std::shared_ptr<GameObject> object, const Coords& sourceCoords, const Coords& targetCoords)
    {
        /*auto& tilet = tile(sourceCoords);
        tilet.removeObject(object);
        tile(targetCoords).addObject(object);*/
    }

private:
    RectangleTileMap map_;
    Coords  playerCoords_;
    GameState gameState_ = GameState::InProgress;
};

//BaseGame::RectangleTileMap loadLevelMap(const std::filesystem::path& path, const GameObjectFactory& objectFactory)
//{
//    auto file = std::ifstream { path };
//    if (!file.is_open()) {
//        throw std::runtime_error("Failed to open path " + path.string());
//    }
//
//    BaseGame::RectangleTileMap map;
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

} // namespace sbg
} // namespace sokoban