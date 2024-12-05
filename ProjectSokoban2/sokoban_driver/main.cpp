// #include "base_session_context.hpp"
#include "sokoban_core/game_state.hpp"
#include "sokoban_core/new_game_parameters.hpp"
#include "sokoban_core/action_logger.hpp"
#include "sokoban_core/mod.hpp"
#include "sokoban_core/default_paths.hpp"
#include "tui/system_init.hpp"
#include "tui/menu_collection.hpp"
#include "tui/menu.hpp"
#include "tui/keyboard.hpp"
#include "tui/key_definitions_win.hpp"
#include "tui/tui.hpp"

#include <boost/dll/import.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>

#include <iostream>

std::filesystem::path showLoadModMenu();
void                  startGame(const std::filesystem::path& modFolderPath);

int main(int argc, char* argv[])
{
    const auto success = sokoban::tui::System::initialize();
    if (!success) {
        return -1;
    }

    try {
        boost::filesystem::path lib_path {
            "C:\\Boo\\Code\\Otus\\ProjectSokoban2\\build\\msvc-debug\\lib_mod_example\\mod_example_lib"
        };

        auto pluginCreatorFunction = boost::dll::import_alias<std::unique_ptr<sokoban::Mod>()>(
            lib_path, "createPlugin", boost::dll::load_mode::append_decorations);

        auto plugin = pluginCreatorFunction();
        // std::cout << plugin->name() << std::endl;
        auto c = plugin->createSessionContext();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }

    sokoban::NewGameParameters newGameParameters;
    newGameParameters.modFolder = sokoban::default_paths::modsFolder / "Core";

    std::filesystem::path        modPath = "Mods/BaseGame";
    sokoban::tui::MenuCollection menus;
    while (true) {
        sokoban::tui::clearConsole();
        sokoban::tui::printMenu(menus.mainMenu);
        startGame(modPath);
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

std::filesystem::path showLoadModMenu()
{
    sokoban::tui::clearConsole();
    std::size_t                                   counter = 0;
    static constexpr std::size_t                  maxMods = 5;
    std::vector<std::filesystem::directory_entry> modFolders;
    for (const auto& dirEntry : std::filesystem::directory_iterator { sokoban::default_paths::modsFolder }) {
        if (dirEntry.is_directory()) {
            std::filesystem::path path { dirEntry };
            std::cout << counter++ + 1 << ": " << path.filename() << '\n';
            modFolders.push_back(dirEntry);
        }
    }

    const auto key = sokoban::tui::waitForInput();
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

class ModDll
{
private: // types
    using ModCreatorFunction = std::unique_ptr<sokoban::Mod>();

public:
    ModDll(const boost::filesystem::path& modDllPath)
        : library_ { boost::make_shared<boost::dll::shared_library>(modDllPath,
                                                                    boost::dll::load_mode::append_decorations) }
        , mod_ { library_->get_alias<ModCreatorFunction>("createMod")() }
    {
    }

    sokoban::Mod& mod() { return *mod_; }

private: // data
    boost::shared_ptr<boost::dll::shared_library> library_ = nullptr;
    std::unique_ptr<sokoban::Mod>                 mod_ = nullptr;
};

ModDll loadModDll(const std::filesystem::path& modFolderPath)
{
    boost::filesystem::path modDllPath = modFolderPath.string() + "/mod";
    return ModDll { modDllPath };
}

void startGame(const std::filesystem::path& modFolderPath)
{
    try {
        ModDll modDll = loadModDll(modFolderPath);
        std::cout << "Mod loaded: " << modDll.mod().name() << '\n';
        auto context = modDll.mod().createSessionContext();
        context->startGame();

    } catch (std::exception e) {
        std::cout << "Exception: " << e.what() << '\n';
    }

    sokoban::tui::waitForInput();
}

// void startGame(const sokoban::NewGameParameters& parameters)
//{
//     sokoban::sbg::BaseSessionContext sessionContext;
//     sessionContext.drawLevel(sessionContext.game().map());
//     bool finished = false;
//     do {
//         sokoban::Key c = sokoban::tui::waitForInput();
//         if (c == sokoban::Key::esc) {
//             return;
//         }
//         finished = sessionContext.executeCommand(std::make_shared<sokoban::Command>(c));
//     } while (!finished);
//
//     system("cls");
//     std::cout << "Yoy won!!!\n";
//     std::cout << "Press any key to return to main window\n";
//     sokoban::tui::waitForInput();
// }