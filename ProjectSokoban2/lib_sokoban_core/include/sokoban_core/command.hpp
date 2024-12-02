#pragma once

#include "tui/key_definitions.hpp"

namespace sokoban {

class Command
{
public:
    Command(sokoban::tui::Key key) : key_ { key } {}

    sokoban::tui::Key key() const {return key_;}

private:
    sokoban::tui::Key key_ = sokoban::tui::Key::invalidKey;
};
} // namespace sokoban