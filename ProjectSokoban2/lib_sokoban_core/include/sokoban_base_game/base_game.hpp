#pragma once

#include "sokoban_core/game.hpp"
#include "sokoban_core/game_action.hpp"
#include "sokoban_core/tile.hpp"
#include "sokoban_core/coords.hpp"
#include "sokoban_core/direction.hpp"

namespace sokoban {
namespace sbg {

class BaseGame : public Game
{
public: // types
    using Row     = std::vector<Tile>;
    using TileMap = std::vector<Row>;

public:
    BaseGame(BaseGame::TileMap map) : map_ { map } {
        for (std::size_t i = 0; i < map_.size(); ++i) {
            for (std::size_t j = 0; j < map_.size(); ++j) {
                const auto& tile = map_[i][j];
                for (const auto& object : tile) {
                    if (object->id()->id() == "player") {
                        playerCoords_ = { j, i };
                        return;
                    }
                }
            }
        }
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
    void setMap(const TileMap& map) { map_ = map; }

    const TileMap& map() const { return map_; }

    TileMap& map() { return map_; }
    std::optional<std::reference_wrapper<const Tile>> adjacentTile(const Coords& coords, Direction direction) const
    {
        if (coords.y() >= map_.size()) {
            return {};
        }
        const auto& row = map_[coords.y()];
        if (direction == Direction::Right && row.size() <= coords.x() - 1) {
            return {};
        }
        if (direction == Direction::Left && coords.x() == 0) {
            return {};
        }
        if (direction == Direction::Up && coords.y() == 0) {
            return {};
        }
        if (direction == Direction::Down && coords.y() >= map_.size() - 1) {
            return {};
        }

        if (direction == Direction::Left) {
            return row[coords.x() - 1];
        }
        if (direction == Direction::Right) {
            return row[coords.x() + 1];
        }
        if (direction == Direction::Up) {
            return map_[coords.y() - 1][coords.x()];
        }
        if (direction == Direction::Down) {
            return map_[coords.y() + 1][coords.x()];
        }

        return {};
    }

    Coords adjacentCoords(const Coords& coords, Direction direction) const
    {
        if (direction == Direction::Left) {
            return { coords.x() - 1, coords.y() };
        }
        if (direction == Direction::Right) {
            return { coords.x() + 1, coords.y() };
        }
        if (direction == Direction::Up) {
            return { coords.x(), coords.y() - 1 };
        }
        if (direction == Direction::Down) {
            return { coords.x(), coords.y() + 1 };
        }

        return {};
    }

    const Tile& tile(const Coords& coords) const { return map_[coords.y()][coords.x()]; }

    Tile& tile(const Coords& coords) { return map_[coords.y()][coords.x()]; }

    const Coords& playerCoords() const { return playerCoords_; }

    void setPlayerCoords(const Coords& coords) { playerCoords_ = coords; }
    void moveObject(std::shared_ptr<GameObject> object, const Coords& sourceCoords, const Coords& targetCoords)
    {
        auto& tilet = tile(sourceCoords);
        tilet.removeObject(object);
        tile(targetCoords).addObject(object);
    }

private:
    TileMap map_;
    Coords  playerCoords_;
    GameState gameState_ = GameState::InProgress;
};

inline BaseGame::TileMap loadLevelMap(const std::filesystem::path& path, const GameObjectFactory& objectFactory)
{
    auto file = std::ifstream { path };
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open path " + path.string());
    }

    BaseGame::TileMap map;

    std::string line;
    while (std::getline(file, line)) {
        BaseGame::Row row(line.size());
        for (std::size_t i = 0; i < line.size(); ++i) {
            row[i].setObjects(objectFactory.create(line[i]));
        }
        map.push_back(std::move(row));
    }

    return map;
}

} // namespace sbg
} // namespace sokoban