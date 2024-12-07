#include "base_game_action.hpp"
#include "sokoban_core/game.hpp"
#include "base_session_context.hpp"

namespace sokoban {
namespace sbg {

std::pair<bool, bool> BaseGameAction::perform(BaseSessionContext& context)
{
    bool success = false;
    switch (key_) {
        case Key::invalidKey: break;
        case Key::digit1: break;
        case Key::digit2: break;
        case Key::digit3: break;
        case Key::digit4: break;
        case Key::digit5: break;
        case Key::w: success = context.game().movePlayer(Direction::Up, context.game()); break;
        case Key::a: success = context.game().movePlayer(Direction::Left, context.game()); break;
        case Key::s: success = context.game().movePlayer(Direction::Down, context.game()); break;
        case Key::d: success = context.game().movePlayer(Direction::Right, context.game()); break;
        case Key::upArrow: success = context.game().movePlayer(Direction::Up, context.game()); break;
        case Key::leftArrow: success = context.game().movePlayer(Direction::Left, context.game()); break;
        case Key::rightArrow: success = context.game().movePlayer(Direction::Right, context.game()); break;
        case Key::downArrow: success = context.game().movePlayer(Direction::Down, context.game()); break;
        case Key::esc: break;
        default: break;
    }

    context.drawLevel(context.game().map());

    return { success, context.game().isFinished() };
}

} // namespace sbg
} // namespace sokoban
