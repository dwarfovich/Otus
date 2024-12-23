#pragma once

#include "sokoban_core/system.hpp"
#include "tui/key_definitions_linux.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <cstdint>

namespace sokoban {
namespace tui {

class Console
{
public:
    Console(System& system) : system_ { system } {}

    void clear() const { std::system("clear"); }
    Key  waitForInput() const
    {
        static fd_set  fdSet;
        while (true) {
            FD_ZERO(&fdSet);
            FD_SET(fileno(stdin), &fdSet);

            int result = select(fileno(stdin) + 1, &fdSet, NULL, NULL, NULL);
            if (result > 0) {
                uint32_t c = 0;
                read(fileno(stdin), &c, sizeof(c));
                return fromVirtualChar(c);
            } else {
                return Key::invalidKey;
            }
        }

        return {};
    }

private:
    System& system_;
};

} // namespace tui
} // namespace sokoban