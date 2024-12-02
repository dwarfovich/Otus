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

bool tryMoveObject(GameObjectSptr object, const Coords& coords, Direction direction, BaseGame& game)
{
    auto&       map          = game.map();
    const auto& nextTile     = game.adjacentTile(coords, direction);
    if (!nextTile.has_value()) {
        return false;
    }

    if(!tileIsMovable(nextTile.value())){
        return false;
    }

    const auto& playerTile = game.tile(coords);
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

bool tryMovePlayer(Direction direction, BaseGame& game)
{
    auto&       map          = game.map();
    const auto& playerCoords = game.playerCoords();
    const auto& nextTile     = game.adjacentTile(playerCoords, direction);
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
            const auto& nextCoords = game.adjacentCoords(playerCoords, direction);
            hasCrate = !tryMoveObject(object, nextCoords, direction, game);  
            break;
        }
    }

    if(hasCrate){
        return false;
    }

    const auto& playerTile = game.tile(game.playerCoords());
    auto        iter = std::find_if(playerTile.objects().begin(), playerTile.objects().end(), [](const auto& iter) {
        return iter->id()->id() == "player";
    });
    if (iter != playerTile.objects().cend()) {
        auto player = *iter;
        if (direction == Direction::Left) {
            game.moveObject(player, playerCoords, { playerCoords.x() - 1, playerCoords.y() });
            game.setPlayerCoords({ playerCoords.x() - 1, playerCoords.y() });
        }
        if (direction == Direction::Right) {
            game.moveObject(player, playerCoords, { playerCoords.x() + 1, playerCoords.y() });
            game.setPlayerCoords({ playerCoords.x() + 1, playerCoords.y() });
        }
        if (direction == Direction::Up) {
            game.moveObject(player, playerCoords, { playerCoords.x(), playerCoords.y() - 1 });
            game.setPlayerCoords({ playerCoords.x(), playerCoords.y() - 1 });
        }
        if (direction == Direction::Down) {
            game.moveObject(player, playerCoords, { playerCoords.x(), playerCoords.y() + 1 });
            game.setPlayerCoords({ playerCoords.x(), playerCoords.y() + 1 });
        }
    }
    return true;
}

bool BaseGameAction::perform(BaseSessionContext& context)
{
    switch (key_) {
        case Key::invalidKey: break;
        case Key::digit1: break;
        case Key::digit2: break;
        case Key::digit3: break;
        case Key::digit4: break;
        case Key::digit5: break;
        case Key::a: tryMovePlayer(Direction::Left, context.game()); break;
        case Key::s: tryMovePlayer(Direction::Down, context.game()); break;
        case Key::d: tryMovePlayer(Direction::Right, context.game()); break;
        case Key::w: tryMovePlayer(Direction::Up, context.game()); break;
        case Key::leftArrow: tryMovePlayer(Direction::Left, context.game()); break;
        case Key::rightArrow: tryMovePlayer(Direction::Right, context.game()); break;
        case Key::upArrow: tryMovePlayer(Direction::Up, context.game()); break;
        case Key::downArrow: tryMovePlayer(Direction::Down, context.game()); break;
        case Key::esc: break;
        default: break;
    }

    context.drawLevel(context.game().map());

    return context.game().isFinished();
}

} // namespace sbg
} // namespace sokoban
