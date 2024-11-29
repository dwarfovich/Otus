#include "sokoban_base_game/base_context_factory.hpp"
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
    sessionContext.loadLevel(parameters.modFolder / "level1.json");
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