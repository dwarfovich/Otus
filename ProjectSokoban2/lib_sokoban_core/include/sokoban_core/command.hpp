#pragma once

#include "key_definitions.hpp"

namespace sokoban {

class Command
{
public:
    Command(Key key) : key_ { key } {}

    Key key() const {return key_;}

private:
    Key key_ = Key::invalidKey;
};

} // namespace sokoban