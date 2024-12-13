#include "mod_dynamic_library.hpp"

#include "sokoban_core/game_state.hpp"
#include "sokoban_core/game.hpp"
#include "sokoban_core/new_game_parameters.hpp"
#include "sokoban_core/action_logger.hpp"
#include "sokoban_core/mod.hpp"
#include "sokoban_core/default_paths.hpp"
#include "sokoban_core/system.hpp"
#include "sokoban_core/player.hpp"
#include "json_utils/json_utils.hpp"
#include "tui/menu_collection.hpp"
#include "tui/menu.hpp"

#include <boost/dll/import.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>

#include <iostream> 

std::filesystem::path showLoadModMenu(const sokoban::tui::Console& console);
void initialize(const std::filesystem::path& modFolderPath, const std::shared_ptr<sokoban::tui::Console>& console, sokoban::Player& player);

int main(int argc, char* argv[])
{
    sokoban::System system;
    auto            console = std::make_shared<sokoban::tui::Console>(system);

    const auto json =sokoban::json_utils::loadFromFile("players.json");
    auto players = sokoban::loadPlayersList(json);


    sokoban::NewGameParameters newGameParameters;
    newGameParameters.modFolder = sokoban::default_paths::modsFolder / "Core";
     
    std::filesystem::path        modPath = "Mods/BaseGame";
    sokoban::tui::MenuCollection menus;
    while (true) {
        console->clear();
        sokoban::tui::printMenu(menus.mainMenu);
        initialize(modPath, console, players[0]);
        /*sokoban::Key c = sokoban::tui::waitForInput();
        if (c == sokoban::Key::esc) {
            return 0;
        }
        switch (c) {
            case sokoban::Key::digit1: startGame(modPath); break;
            case sokoban::Key::digit2: break;
            case sokoban::Key::digit3: {
                const auto& selectedPath = showLoadModMenu();
                if (!selectedPath.empty()) {
                    modPath = selectedPath;
                }
                break;
            }
            case sokoban::Key::digit4: break;
            case sokoban::Key::digit5: return 0;
            default: break;
        }*/
    }

    return 0;
}

std::filesystem::path showLoadModMenu(const sokoban::tui::Console& console)
{
    console.clear();
    std::size_t                                   counter = 0;
    static constexpr std::size_t                  maxMods = 5;
    std::vector<std::filesystem::directory_entry> modFolders;
    for (const auto& dirEntry : std::filesystem::directory_iterator { sokoban::default_paths::modsFolder }) {
        if (dirEntry.is_directory()) {
            std::filesystem::path path { dirEntry };
            std::cout << ++counter + 1 << ": " << path.filename() << '\n';
            modFolders.push_back(dirEntry);
        }
    }

    const auto key = console.waitForInput();
    switch (key) {
        case sokoban::Key::digit1: return modFolders[0];
        case sokoban::Key::digit2: return modFolders[1];
        case sokoban::Key::digit3: return modFolders[2];
        case sokoban::Key::digit4: return modFolders[3];
        case sokoban::Key::digit5: return modFolders[4];
        case sokoban::Key::esc: return {};
        default: return {};
    }
}

void initialize(const std::filesystem::path&                  modFolderPath,
               const std::shared_ptr<sokoban::tui::Console>& console,
               sokoban::Player&                              player)
{
    try {
        ModDynamicLibrary modDll = loadModDll(modFolderPath);
        std::cout << "Mod loaded: " << modDll.mod().name() << '\n';
        auto context = modDll.mod().createSessionContext();
        context->setConsole(console);
        context->setModFolderPath(modFolderPath);
        context->initialize();
        bool hasNextLevel = true;
        do {
            context->loadNextLevel();
            sokoban::GameState gameState = sokoban::GameState::InProgress;
            do {
                sokoban::Key c = console->waitForInput();
                if (c == sokoban::Key::esc) {
                    return;
                }
                auto [success, newGameState] = context->executeCommand(std::make_shared<sokoban::Command>(c));
                gameState                    = newGameState;
            } while (gameState == sokoban::GameState::InProgress);

            if (gameState == sokoban::GameState::Won) {
                hasNextLevel = context->hasNextLevel();
                if (hasNextLevel) {
                    context->incrementLevelNumber();
                    std::cout << "You solved this puzzle!!!\n";
                } else{
                    auto message = context->achievement();
                    if(!message.empty()){
                        std::cout << "You got an achievement: " << message << '\n';
                        player.addAchievement(modFolderPath, std::move(message));
                    }
                }
                    std::cout << "Press any key to continue.\n";
            } else {
                std::cout << "Wou lost! Press any key to continue.\n";
            }
            console->waitForInput();
        } while (hasNextLevel);
        std::cout << "Game over. Press any key to continue.\n";
        console->waitForInput();
    } catch (std::exception e) {
        std::cout << "Exception: " << e.what() << '\n';
    }
}
