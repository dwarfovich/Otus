#pragma once

#include "menu.hpp"
#include "main_menu_entries_id.hpp"

namespace sokoban {
namespace tui {

class MenuCollection
{
public:
    Menu<MainMenuEntriesId> mainMenu { { MenuEntry<MainMenuEntriesId> { "Play", MainMenuEntriesId::Play },
                                         { "Load game", MainMenuEntriesId::LoadGame },
                                         { "Load mod", MainMenuEntriesId::LoadMod },
                                         { "Options", MainMenuEntriesId::Options },
                                         { "Exit", MainMenuEntriesId::Exit } } };
};

} // namespace tui
} // namespace sokoban