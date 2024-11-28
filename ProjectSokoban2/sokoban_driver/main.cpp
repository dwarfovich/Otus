#include "sokoban_base_game/base_context_factory.hpp"
#include "sokoban_core/game_state.hpp"
#include "tui/system_init.hpp"
#include "tui/menu_collection.hpp"
#include "tui/menu.hpp"
#include "tui/keyboard.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    const auto success = sokoban::tui::System::initialize();
    if(!success){
        return -1;
    }

    sokoban::tui::MenuCollection menus;
    sokoban::tui::printMenu(menus.mainMenu);
    while(true){
        char c = sokoban::tui::waitForInput();
        if (c == 0){
            return 0;
        }
        std::cout << int(c) << ' ' << c << '\n';
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