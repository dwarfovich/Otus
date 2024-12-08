#pragma once

#include "keyboard.hpp"

namespace sokoban {
namespace tui {

class Console
{
public:
    Console(System& system) : system_ { system } {}

    void clear() const { system("cls"); }
    Key  waitForInput() const
    {
        static INPUT_RECORD inputRecord;
        static DWORD        eventsRead = 0;
        while (true) {
            auto success = ReadConsoleInput(system_.inputHandle(), &inputRecord, 1, &eventsRead);
            if (!success) {
                return Key::invalidKey;
            }
            if (inputRecord.EventType == KEY_EVENT) {
                const auto& event = inputRecord.Event.KeyEvent;
                if (event.bKeyDown) {
                    return fromVirtualWord(event.wVirtualKeyCode);
                }
            }
        }
    }

private:
    System& system_;
};

} // namespace tui
} // namespace sokoban