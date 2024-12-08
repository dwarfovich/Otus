#include "base_game_action.hpp"
#include "sokoban_core/game.hpp"
#include "base_session_context.hpp"

namespace sokoban {
namespace sbg {

std::pair<bool, bool> BaseGameAction::perform(BaseGame& context)
{
    bool success = false;
    switch (key_) {
        case Key::invalidKey: break;
        case Key::digit1: break;
        case Key::digit2: break;
        case Key::digit3: break;
        case Key::digit4: break;
        case Key::digit5: break;
        case Key::w: success = context.movePlayer(Direction::Up); break;
        case Key::a: success = context.movePlayer(Direction::Left); break;
        case Key::s: success = context.movePlayer(Direction::Down); break;
        case Key::d: success = context.movePlayer(Direction::Right); break;
        case Key::upArrow: success = context.movePlayer(Direction::Up); break;
        case Key::leftArrow: success = context.movePlayer(Direction::Left); break;
        case Key::rightArrow: success = context.movePlayer(Direction::Right); break;
        case Key::downArrow: success = context.movePlayer(Direction::Down); break;
        case Key::esc: break;
        default: break;
    }

    

    return { success, context.isFinished() };
}

} // namespace sbg
} // namespace sokoban
