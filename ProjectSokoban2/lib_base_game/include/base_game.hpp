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
    BaseGame(RectangleTileMap map, const Coords& playerCoords) : map_ { map }, playerCoords_{playerCoords}
    {
    }

    bool isFinished() const
    {
        for (const auto& row : map_) {
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
        }

        return true;
    }
    void setMap(const RectangleTileMap& map) { map_ = map; }

    const RectangleTileMap& map() const { return map_; }

    RectangleTileMap& map() { return map_; }

    const Tile& tile(const Coords& coords) const { return map_.map()[coords.y()][coords.x()]; }


    const Coords& playerCoords() const { return playerCoords_; }

    void setPlayerCoords(const Coords& coords) { playerCoords_ = coords; }
    void moveObject(std::shared_ptr<GameObject> object, const Coords& sourceCoords, const Coords& targetCoords)
    {
       map_.moveObject(object, sourceCoords, targetCoords);
    }

    bool tileIsMovable(const Tile& tile)
    {
        for (const auto& object : tile) {
            if ((*object->id() == "player") || (*object->id() == "crate") || (*object->id() == "wall")) {
                return false;
            }
        }

        return true;
    }

     bool moveObject(GameObjectSptr object, const Coords& coords, Direction direction)
    {
        const auto& nextTile = map_.adjacentTile(coords, direction);
        if (!nextTile.has_value()) {
            return false;
        }
    
        if (!tileIsMovable(nextTile.value())) {
            return false;
        }
    
        const auto& sourceTile = map_.tile(coords);
        auto        iter       = std::find(sourceTile.objects().begin(), sourceTile.objects().end(), object);
        if (iter != sourceTile.objects().cend()) {
            auto nextCoords = map_.adjacentCoords(coords, direction);
            if(nextCoords.has_value()){
                map_.moveObject(object, coords, nextCoords.value() );
                return true;
            }
        }
        return false;
    }

      bool movePlayer(Direction direction, BaseGame& game)
    {
        const auto& nextTile     = map_.adjacentTile(playerCoords_, direction);
        if (!nextTile.has_value()) {
            return false;
        }
    
        const auto& tile = nextTile.value();
        for (const auto& object : tile.get()) {
            if (object->id()->id() == "wall") {
                return false;
            }
        }
    
        bool hasCrate = false;
        for (const auto& object : tile.get()) {
            if (object->id()->id() == "crate") {
                const auto& nextCoords = map_.adjacentCoords(playerCoords_, direction);
                hasCrate               = !moveObject(object, nextCoords.value(), direction);
                break;
            }
        }
    
        if (hasCrate) {
            return false;
        }
    
        const auto& playerTile = map_.tile(playerCoords_);
        auto        iter = std::find_if(playerTile.objects().begin(), playerTile.objects().end(), [](const auto& iter)
        {
            return iter->id()->id() == "player";
        });
        if (iter != playerTile.objects().cend()) {
            auto player = *iter;
            const auto& nextCoords = map_.adjacentCoords(playerCoords_, direction);
            if(!nextCoords.has_value()){
                return false;
            }
            bool success = moveObject(player, playerCoords_, direction);
            if(success){
                playerCoords_ = nextCoords.value();
                return true;
            }
        }
        return false;
    }

private:
    RectangleTileMap map_;
    Coords  playerCoords_;
    GameState gameState_ = GameState::InProgress;
};

inline std::pair<RectangleTileMap, Coords> loadLevelMap(const std::filesystem::path& path, const GameObjectFactory& objectFactory)
{
    auto file = std::ifstream { path };
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open path " + path.string());
    }

    std::vector<RectangleTileMap::Row> map;
    std::optional<Coords> playerCoords_;
    std::string line;
    std::size_t rowsCount = 0;
    while (std::getline(file, line)) {
        RectangleTileMap::Row row(line.size());
        for (std::size_t i = 0; i < line.size(); ++i) {
            auto objects = objectFactory.create(line[i]);
            for(const auto& object : objects){
                if (*object->id() == "player"){
                    playerCoords_ = {i, rowsCount};
                }
            }
            row[i].setObjects(objectFactory.create(line[i]));
        }
        map.push_back(std::move(row));
        ++rowsCount;
    }

    return {RectangleTileMap{std::move(map)}, playerCoords_.value()};
}

} // namespace sbg
} // namespace sokoban