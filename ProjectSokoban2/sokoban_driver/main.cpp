#include "sokoban_base_game/base_session_context.hpp"
#include "sokoban_core/game_state.hpp"
#include "sokoban_core/new_game_parameters.hpp"
#include "sokoban_core/action_logger.hpp"
#include "tui/system_init.hpp"
#include "tui/menu_collection.hpp"
#include "tui/menu.hpp"
#include "tui/keyboard.hpp"
#include "tui/key_definitions_win.hpp"

#include <iostream>

void startGame(const sokoban::NewGameParameters& parameters)
{
    sokoban::sbg::BaseSessionContext sessionContext;
    sessionContext.drawLevel(sessionContext.game().map());
    bool finished = false;
    do{
        sokoban::Key c = sokoban::tui::waitForInput();
        if (c == sokoban::Key::esc) {
            return;
        }
        finished = sessionContext.executeCommand(std::make_shared<sokoban::Command>(c));
    } while(!finished);

    system("cls");
    std::cout << "Yoy won!!!\n";
    std::cout << "Press any key to return to main window\n";
    sokoban::tui::waitForInput();
}

int main(int argc, char* argv[])
{
    const auto success = sokoban::tui::System::initialize();
    if(!success){
        return -1;
    }

    sokoban::NewGameParameters newGameParameters;
    newGameParameters.modFolder = sokoban::default_paths::addonsFolder/ "Core";

    sokoban::tui::MenuCollection menus;
    while(true){
        sokoban::tui::printMenu(menus.mainMenu);
        sokoban::Key c = sokoban::tui::waitForInput();
        if (c == sokoban::Key::esc) {
            return 0;
        }
        switch (c) {
            case sokoban::Key::digit1: startGame(newGameParameters); break;
            case sokoban::Key::digit2: break;
            case sokoban::Key::digit3: break;
            case sokoban::Key::digit4: break;
            case sokoban::Key::digit5: return 0;
            default: break;
        }
    }

    return 0;
}