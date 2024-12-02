#pragma once

#include "level_map.hpp"
#include "game_state.hpp"
#include "coords.hpp"

namespace sokoban {

    class Game
{
public:
    Game(LevelMap::TileMap map): map_{map}{}
    virtual ~Game() = default;

    void setMap(const LevelMap& map){
        map_ = map;
    }

    const LevelMap& map() const{
        return map_;
    }

        LevelMap& map() { return map_; }

    void moveObject( std::shared_ptr<GameObject> object, const Coords& sourceCoords, const Coords& targetCoords){
        auto& tile = map_.tile(sourceCoords);
        tile.removeObject(object);
        map_.tile(targetCoords).addObject(object);
    }

protected:
    GameState gameState_ = GameState::InProgress;
    LevelMap map_;
};

} // namespace sc