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
#include <thread>

using namespace sokoban;

std::filesystem::path showLoadModMenu(const sokoban::tui::Console& console);

std::unique_ptr<SessionContext> initializeContext(ModDynamicLibrary&                   modLibrary,
                                                  const std::shared_ptr<Player>&       player,
                                                  const std::shared_ptr<tui::Console>& console);

void saveBaseGameInfo(std::ostream& stream, const sokoban::SessionContext& context, const sokoban::Player& player);

std::filesystem::path showLoadGameMenu(tui::Console& console);

std::pair<sokoban::GameState, bool> playLevel(SessionContext& context, tui::Console& console);

void playGame(SessionContext& context, Player& player, tui::Console& console);

void startNewGame(ModDynamicLibrary&                   modLibrary,
                  const std::shared_ptr<Player>&       player,
                  const std::shared_ptr<tui::Console>& console);

void loadGame(ModDynamicLibrary&                          modLibrary,
              const std::vector<std::shared_ptr<Player>>& players,
              const std::shared_ptr<tui::Console>&        console);

void playLog(ModDynamicLibrary& modLibrary, const std::shared_ptr<tui::Console>& console);

std::filesystem::path generateLogFilePath(const SessionContext& context);

int main(int argc, char* argv[])
{
    sokoban::System system;
    auto            console = std::make_shared<sokoban::tui::Console>(system);

    const auto playersJson = sokoban::json_utils::loadFromFile("players.json");
    const auto players     = sokoban::loadPlayersList(playersJson);

    ModDynamicLibrary            modLibrary = loadModDynamicLibrary("Mods/BaseGame");
    std::filesystem::path        modPath    = "Mods/BaseGame";
    sokoban::tui::MenuCollection menus;
    while (true) {
        console->clear();
        std::cout << "Loaded mod: " << modLibrary.mod().name() << '\n';
        sokoban::tui::printMenu(menus.mainMenu);
        sokoban::Key c = console->waitForInput();
        if (c == sokoban::Key::esc) {
            return 0;
        }
        switch (c) {
            case sokoban::Key::digit1: {
                startNewGame(modLibrary, players[0], console);
                break;
            }
            case sokoban::Key::digit2: {
                loadGame(modLibrary, players, console);
                break;
            }
            case sokoban::Key::digit3: {
                const auto& selectedPath = showLoadModMenu(*console);
                if (!selectedPath.empty()) {
                    modLibrary = loadModDynamicLibrary(selectedPath);
                }
                break;
            }
            case sokoban::Key::digit4: {
                playLog(modLibrary, console);
                break;
            }
            case sokoban::Key::digit5: return 0;
            default: break;
        }
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
            std::cout << ++counter << ": " << path.filename() << '\n';
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

std::unique_ptr<SessionContext> initializeContext(ModDynamicLibrary&                   modLibrary,
                                                  const std::shared_ptr<Player>&       player,
                                                  const std::shared_ptr<tui::Console>& console)
{
    auto context = modLibrary.mod().createSessionContext();
    if(!context){
        throw std::runtime_error("Failed to create context");
    }
    context->setConsole(console);
    context->setModFolderPath("");
    context->setModFolderPath(modLibrary.modFolderPath());
    context->setPlayer(player);
    context->initialize();

    return context;
}

std::filesystem::path generateSaveGameFilePath(const std::filesystem::path& modFolder)
{
    return default_paths::saveGameFolder / "savegame1.sav";
}

void playGame(SessionContext& context, Player& player, tui::Console& console)
{
    try {
        bool hasNextLevel = true;
        while (hasNextLevel) {
            auto [gameState, abortRequested] = playLevel(context, console);
            if (abortRequested) {
                return;
            }
            if (gameState == sokoban::GameState::Won) {
                std::cout << "You solved this puzzle!!!\n";
                hasNextLevel = context.hasNextLevel();
                if (hasNextLevel) {
                    context.incrementLevelNumber();
                    context.loadLevel();
                } else {
                    std::cout << "Game over!\n";
                    auto message = context.achievement();
                    if (!message.empty()) {
                        std::cout << "You got an achievement: " << message << '\n';
                        player.addAchievement(context.modFolderPath(), std::move(message));
                    }
                }
                std::cout << "Press any key to continue.\n";
            } else {
                std::cout << "Wou lost! Press any key to continue.\n";
            }
            console.waitForInput();
        }

    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
        console.waitForInput();
    }
}

std::pair<sokoban::GameState, bool> playLevel(SessionContext& context, tui::Console& console)
{
    std::ofstream logStream { generateLogFilePath(context) };
    logStream << context.currentLevelPath() << '\n';

    sokoban::GameState gameState = sokoban::GameState::InProgress;
    do {
        context.drawLevel();
        sokoban::Key c = console.waitForInput();
        if (c == sokoban::Key::esc) {
            return { {}, true };
        } else if (c == Key::digit5) {
            std::filesystem::create_directories(default_paths::saveGameFolder);
            const auto&   filePath = generateSaveGameFilePath("SaveGame1.sv");
            std::ofstream stream { filePath };
            if (stream.is_open()) {
                stream << context.player()->name() << '\n';
                stream << context.modFolderPath() << '\n';
                context.saveGame(stream);
            }
        } else {
            auto [success, newGameState] = context.executeCommand(std::make_shared<sokoban::Command>(c));
            if (success) {
                logStream << static_cast<std::underlying_type_t<Key>>(c) << '\n';
            }
            gameState = newGameState;
        }
    } while (gameState == sokoban::GameState::InProgress);

    return { gameState, false };
}

void saveBaseGameInfo(std::ostream& stream, const sokoban::SessionContext& context, const sokoban::Player& player)
{
    stream << player.name() << '\n';
    stream << context.modFolderPath().string() << '\n';
}

std::filesystem::path showLoadGameMenu(tui::Console& console)
{
    console.clear();
    std::size_t                                   counter = 0;
    static constexpr std::size_t                  maxMods = 5;
    std::vector<std::filesystem::directory_entry> modFolders;
    for (const auto& dirEntry : std::filesystem::directory_iterator { sokoban::default_paths::saveGameFolder }) {
        if (!dirEntry.is_directory()) {
            std::filesystem::path path { dirEntry };
            std::cout << ++counter << ": " << path.filename() << '\n';
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

void startNewGame(ModDynamicLibrary&                   modLibrary,
                  const std::shared_ptr<Player>&       player,
                  const std::shared_ptr<tui::Console>& console)
{
    auto context = initializeContext(modLibrary, player, console);
    context->loadLevel();
    playGame(*context, *player, *console);
}

void loadGame(ModDynamicLibrary&                          modLibrary,
              const std::vector<std::shared_ptr<Player>>& players,
              const std::shared_ptr<tui::Console>&        console)
{
    auto          saveGamePath = showLoadGameMenu(*console);
    std::ifstream stream { saveGamePath };
    if (!stream.is_open()) {
        return;
    }

    std::string playerName;
    std::getline(stream, playerName);
    std::string modFolderStr;
    std::getline(stream, modFolderStr);
    modFolderStr.pop_back();
    modFolderStr.erase(0, 1);
    std::filesystem::path modFolder { modFolderStr };
    try {
        modLibrary = loadModDynamicLibrary(modFolder);
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }
    std::size_t playerIndex = std::numeric_limits<std::size_t>::max();
    for (std::size_t i = 0; i < players.size(); ++i) {
        if (players[i]->name() == playerName) {
            playerIndex = i;
            break;
        }
    }
    if (playerIndex == std::numeric_limits<std::size_t>::max()) {
        return;
    }

    auto context = initializeContext(modLibrary, players[playerIndex], console);
    context->loadGame(stream);
    playGame(*context, *players[playerIndex], *console);
}

std::filesystem::path generateLogFilePath(const SessionContext& context)
{
    std::filesystem::path path { context.modFolderPath() / "Logs" };
    std::filesystem::create_directories(path);
    path /= "log";

    std::time_t time { std::time(nullptr) };
    char        timeStringBuffer[100];
    if (std::strftime(timeStringBuffer, sizeof(timeStringBuffer), "-%Y-%m-%d_%H_%M_%S", std::localtime(&time))) {
        path += timeStringBuffer;
    }
    path += ".gl";

    return path;
}

std::filesystem::path selectLog(const std::filesystem::path& modFolder, tui::Console& console)
{
    std::vector<std::filesystem::path> logs;

    std::size_t                                   counter = 0;
    static constexpr std::size_t                  maxLogs = 5;
    std::vector<std::filesystem::directory_entry> paths;
    auto                                          t = modFolder / "Logs";
    for (const auto& dirEntry : std::filesystem::directory_iterator { modFolder / "Logs" }) {
        if (!dirEntry.is_directory()) {
            std::filesystem::path path { dirEntry };
            std::cout << ++counter << ": " << path.filename() << '\n';
            paths.push_back(dirEntry);
        }
        if (counter >= 5) {
            break;
        }
    }

    const auto c = console.waitForInput();
    switch (c) {
        case sokoban::Key::digit1: return paths[0];
        case sokoban::Key::digit2: return paths[1];
        case sokoban::Key::digit3: return paths[2];
        case sokoban::Key::digit4: return paths[3];
        case sokoban::Key::digit5: return paths[4];
        case sokoban::Key::esc: return {};
        default: return {};
    }
}

void playLog(ModDynamicLibrary& modLibrary, const std::shared_ptr<tui::Console>& console)
{
    const auto modFolder = modLibrary.modFolderPath();
    const auto logPath   = selectLog(modFolder, *console);
    if (logPath.empty()) {
        return;
    }

    auto context = initializeContext(modLibrary, nullptr, console);
    context->loadLevel();

    std::ifstream logStream { logPath };
    if (!logStream.is_open()) {
        return;
    }

    std::filesystem::path levelPath;
    logStream >> levelPath;

    std::underlying_type_t<Key> keyValue;

    context->loadLevel(levelPath);
    context->drawLevel();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    while (logStream >> keyValue) {
        Key key { keyValue };
        context->executeCommand(std::make_shared<sokoban::Command>(key));
        context->drawLevel();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "Finished. Press any key to return to main menu.\n";
    console->waitForInput();
}