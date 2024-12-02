#include "sokoban_base_game/base_session_context.hpp"
#include "sokoban_core/game_state.hpp"
#include "sokoban_core/new_game_parameters.hpp"
#include "tui/system_init.hpp"
#include "tui/menu_collection.hpp"
#include "tui/menu.hpp"
#include "tui/keyboard.hpp"
#include "tui/key_definitions_win.hpp"

#include <iostream>

void startGame(const sokoban::NewGameParameters& parameters)
{
    sokoban::sbg::BaseSessionContext sessionContext;
    sessionContext.drawLevel(sessionContext.level());
    bool finished = false;
    do{
        sokoban::tui::Key c = sokoban::tui::waitForInput();
        if (c == sokoban::tui::Key::esc) {
            return;
        }
        finished = sessionContext.executeCommand(std::make_shared<sokoban::Command>(c));
    } while(!finished);

    system("clear");
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
    sokoban::tui::printMenu(menus.mainMenu);
    while(true){
        //sokoban::tui::Key c = sokoban::tui::waitForInput();
        sokoban::tui::Key c = sokoban::tui::Key::digit1;
        if (c == sokoban::tui::Key::esc) {
            return 0;
        }
        switch (c) {
            case sokoban::tui::Key::digit1: startGame(newGameParameters); break;
            case sokoban::tui::Key::digit2: break;
            case sokoban::tui::Key::digit3: break;
            case sokoban::tui::Key::digit4: break;
            case sokoban::tui::Key::digit5: break;
            default: break;
        }

        break;
    }

    return 0;
}

 // MAIN LOOP
// show GameResult;
/*
* Campaign c;
* {
* sokoban::GameState gameState;
while(gameState != sokoban::GameState::Finished){
    // read input
    // actionResult = sessionContext.executeCommand(inputCommand);
    // sessionContext.representActionResult(actionResult);
    // gameState = actionResult.newState;
}

// show GameResult;
}
show main menu
*/