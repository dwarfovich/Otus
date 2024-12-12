#include "base_game_action.hpp"
#include "sokoban_core/game.hpp"
#include "base_session_context.hpp"

namespace sokoban {
namespace sbg {

std::pair<bool, GameState> BaseGameAction::perform(BaseGame& game)
{
    bool success = false;
    switch (key_) {
        case Key::invalidKey: break;
        case Key::digit1: break;
        case Key::digit2: break;
        case Key::digit3: break;
        case Key::digit4: break;
        case Key::digit5: break;
        case Key::w: success = game.movePlayer(Direction::Up); break;
        case Key::a: success = game.movePlayer(Direction::Left); break;
        case Key::s: success = game.movePlayer(Direction::Down); break;
        case Key::d: success = game.movePlayer(Direction::Right); break;
        case Key::upArrow: success = game.movePlayer(Direction::Up); break;
        case Key::leftArrow: success = game.movePlayer(Direction::Left); break;
        case Key::rightArrow: success = game.movePlayer(Direction::Right); break;
        case Key::downArrow: success = game.movePlayer(Direction::Down); break;
        case Key::esc: break;
        default: break;
    }

    return { success, game.gameState() };
}

} // namespace sbg
} // namespace sokoban
