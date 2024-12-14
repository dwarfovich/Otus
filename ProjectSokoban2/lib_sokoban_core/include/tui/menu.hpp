#pragma once

#include "menu_entry.hpp"

#include <iostream>
#include <array>
#include <string>

namespace sokoban {
namespace tui {

template<typename Enum>
class Menu
{
public:
    Menu(const std::array<MenuEntry<Enum>, static_cast<size_t>(Enum::idsCount)>& entries) : entries_ { entries } {}

    Enum               getValue(uint8_t index) { return entries_[index]; }
    uint8_t            size() const { return static_cast<uint8_t>(entries_.size()); }
    const std::string& text(uint8_t index) const { return entries_[index].text; }

private:
    std::array<MenuEntry<Enum>, static_cast<size_t>(Enum::idsCount)> entries_;
};

template<typename MenuType>
void printMenu(const MenuType& menu)
{
    for (uint8_t i = 0; i < menu.size(); ++i) {
        std::cout << +i + 1 << ": " << menu.text(i) << '\n';
    }
    std::cout << "Choose entry number...\n";
}

} // namespace tui
} // namespace sokoban