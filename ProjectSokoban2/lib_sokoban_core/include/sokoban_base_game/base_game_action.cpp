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

bool tryMovePlayer(Direction direction, Game& game)
{
    const auto& map = game.map();
    const auto& playerCoords = game.map().playerCoords();
    const auto& nextTile = map.adjacentTile(playerCoords, direction);
    if(nextTile.has_value()){
        const auto& tile = nextTile.value();
        if(!tileIsMovable(tile)){
            return false;
        }
    }
    const auto& playerTile = game.map().tile(game.map().playerCoords());
    auto iter = std::find_if(playerTile.objects().begin(), playerTile.objects().end(), [](const auto& iter){
           return iter->id()->id() == "player"; 
        });
    if (iter != playerTile.objects().cend()){
        auto player = *iter;
        game.moveObject(player, playerCoords, { playerCoords.x() + 1, playerCoords.y()});
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
        case sokoban::tui::Key::a: break;
        case sokoban::tui::Key::s: break;
        case sokoban::tui::Key::d: tryMovePlayer(Direction::Right, context.game()); break;
        case sokoban::tui::Key::w: break;
        case sokoban::tui::Key::leftArrow: break;
        case sokoban::tui::Key::rightArrow: tryMovePlayer(Direction::Right, context.game()); break;
        case sokoban::tui::Key::upArrow: break;
        case sokoban::tui::Key::downArrow: break;
        case sokoban::tui::Key::esc: break;
        default: break;
    }

    context.drawLevel(context.level());
}

} // namespace sbg
} // namespace sokoban
