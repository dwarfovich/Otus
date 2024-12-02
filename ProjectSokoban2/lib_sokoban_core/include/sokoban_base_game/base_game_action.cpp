#include "base_game_action.hpp"
#include "game.hpp"
#include "base_session_context.hpp"

namespace sokoban {
namespace sbg {

bool tileIsMovable(const Tile& tile)
{
    for (const auto& object : tile) {
        if ((object->id()->id() == "player") || (object->id()->id() == "crate") || (object->id()->id() == "wall")) {
            return false;
        }
    }

    return true;
}

bool tryMoveObject(GameObjectSptr object, const Coords& coords, Direction direction, Game& game)
{
    auto&       map          = game.map();
    const auto& nextTile     = map.adjacentTile(coords, direction);
    if (!nextTile.has_value()) {
        return false;
    }

    if(!tileIsMovable(nextTile.value())){
        return false;
    }

    const auto& playerTile = game.map().tile(coords);
    auto        iter = std::find(playerTile.objects().begin(), playerTile.objects().end(), object);
    if (iter != playerTile.objects().cend()) {
        if (direction == Direction::Left) {
            game.moveObject(object, coords, { coords.x() - 1, coords.y() });
        }
        if (direction == Direction::Right) {
            game.moveObject(object, coords, { coords.x() + 1, coords.y() });
        }
        if (direction == Direction::Up) {
            game.moveObject(object, coords, { coords.x(), coords.y() - 1 });
        }
        if (direction == Direction::Down) {
            game.moveObject(object, coords, { coords.x(), coords.y() + 1 });
        }
    }
    return true;
}

bool tryMovePlayer(Direction direction, Game& game)
{
    auto&       map          = game.map();
    const auto& playerCoords = game.map().playerCoords();
    const auto& nextTile     = map.adjacentTile(playerCoords, direction);
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
    for(const auto& object : tile.get()){
        if(object->id()->id() == "crate"){
            const auto& nextCoords = map.adjacentCoords(playerCoords, direction);
            hasCrate = !tryMoveObject(object, nextCoords, direction, game);  
            break;
        }
    }

    if(hasCrate){
        return false;
    }

    const auto& playerTile = game.map().tile(game.map().playerCoords());
    auto        iter = std::find_if(playerTile.objects().begin(), playerTile.objects().end(), [](const auto& iter) {
        return iter->id()->id() == "player";
    });
    if (iter != playerTile.objects().cend()) {
        auto player = *iter;
        if (direction == Direction::Left) {
            game.moveObject(player, playerCoords, { playerCoords.x() - 1, playerCoords.y() });
            map.setPlayerCoords({ playerCoords.x() - 1, playerCoords.y() });
        }
        if (direction == Direction::Right) {
            game.moveObject(player, playerCoords, { playerCoords.x() + 1, playerCoords.y() });
            map.setPlayerCoords({ playerCoords.x() + 1, playerCoords.y() });
        }
        if (direction == Direction::Up) {
            game.moveObject(player, playerCoords, { playerCoords.x(), playerCoords.y() - 1 });
            map.setPlayerCoords({ playerCoords.x(), playerCoords.y() - 1 });
        }
        if (direction == Direction::Down) {
            game.moveObject(player, playerCoords, { playerCoords.x(), playerCoords.y() + 1 });
            map.setPlayerCoords({ playerCoords.x(), playerCoords.y() + 1 });
        }
    }
    return true;
}

void BaseGameAction::perform(BaseSessionContext& context)
{
    switch (key_) {
        case sokoban::tui::Key::invalidKey: break;
        case sokoban::tui::Key::digit1: break;
        case sokoban::tui::Key::digit2: break;
        case sokoban::tui::Key::digit3: break;
        case sokoban::tui::Key::digit4: break;
        case sokoban::tui::Key::digit5: break;
        case sokoban::tui::Key::a: tryMovePlayer(Direction::Left, context.game()); break;
        case sokoban::tui::Key::s: tryMovePlayer(Direction::Down, context.game()); break;
        case sokoban::tui::Key::d: tryMovePlayer(Direction::Right, context.game()); break;
        case sokoban::tui::Key::w: tryMovePlayer(Direction::Up, context.game()); break;
        case sokoban::tui::Key::leftArrow: tryMovePlayer(Direction::Left, context.game()); break;
        case sokoban::tui::Key::rightArrow: tryMovePlayer(Direction::Right, context.game()); break;
        case sokoban::tui::Key::upArrow: tryMovePlayer(Direction::Up, context.game()); break;
        case sokoban::tui::Key::downArrow: tryMovePlayer(Direction::Down, context.game()); break;
        case sokoban::tui::Key::esc: break;
        default: break;
    }

    context.drawLevel(context.level());
}

} // namespace sbg
} // namespace sokoban
