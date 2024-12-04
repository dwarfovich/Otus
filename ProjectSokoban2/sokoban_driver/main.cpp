#include "sokoban_base_game/base_session_context.hpp"
#include "sokoban_core/game_state.hpp"
#include "sokoban_core/new_game_parameters.hpp"
#include "sokoban_core/action_logger.hpp"
#include "sokoban_core/mod.hpp"
#include "tui/system_init.hpp"
#include "tui/menu_collection.hpp"
#include "tui/menu.hpp"
#include "tui/keyboard.hpp"
#include "tui/key_definitions_win.hpp"

#include <boost/dll/import.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>

#include <iostream>

//void startGame(const sokoban::NewGameParameters& parameters)
//{
//    sokoban::sbg::BaseSessionContext sessionContext;
//    sessionContext.drawLevel(sessionContext.game().map());
//    bool finished = false;
//    do {
//        sokoban::Key c = sokoban::tui::waitForInput();
//        if (c == sokoban::Key::esc) {
//            return;
//        }
//        finished = sessionContext.executeCommand(std::make_shared<sokoban::Command>(c));
//    } while (!finished);
//
//    system("cls");
//    std::cout << "Yoy won!!!\n";
//    std::cout << "Press any key to return to main window\n";
//    sokoban::tui::waitForInput();
//}

int main(int argc, char* argv[])
{
    try {
        boost::filesystem::path lib_path {
            "C:\\Boo\\Code\\Otus\\ProjectSokoban2\\build\\msvc-debug\\lib_mod_example\\mod_example_lib"
        };

        auto pluginCreatorFunction = boost::dll::import_alias<std::unique_ptr<sokoban::Mod>()>(
            lib_path,
            "createPlugin",
            boost::dll::load_mode::append_decorations );

        auto plugin = pluginCreatorFunction();
        std::cout << plugin->name() << std::endl;
        auto c = plugin->createSessionContext();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }

    // plugin = boost::dll::import<sokoban::Mod>( // type of imported symbol is located between `<` and `>`
    //      lib_path,        // path to the library and library name
    //      "ModExample"                          // name of the symbol to import
    //);

    // boost::shared_ptr<my_plugin_api> plugin;            // variable to hold a pointer to plugin variable
    // std::cout << "Loading the plugin" << std::endl;

    // plugin = dll::import <my_plugin_api>(  // type of imported symbol is located between `<` and `>`
    //     lib_path / "my_plugin_sum",        // path to the library and library name
    //     "plugin",                          // name of the symbol to import
    //     dll::load_mode::append_decorations // makes `libmy_plugin_sum.so` or `my_plugin_sum.dll` from `my_plugin_sum`
    //);

    /*const auto success = sokoban::tui::System::initialize();
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
    }*/

    return 0;
}