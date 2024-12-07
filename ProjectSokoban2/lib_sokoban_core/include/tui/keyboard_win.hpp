#pragma once

#include "system_init.hpp"
#include "key_definitions_win.hpp"

#include <windows.h>

namespace sokoban {
namespace tui {

inline Key waitForInput()
{
    static INPUT_RECORD inputRecord;
    static DWORD        eventsRead = 0;
    while(true) {
        auto success = ReadConsoleInput(System::inputHandle(), &inputRecord, 1, &eventsRead);
        if (!success) {
            return Key::invalidKey;
        }
        if (inputRecord.EventType == KEY_EVENT){
            const auto& event = inputRecord.Event.KeyEvent;
            if(event.bKeyDown){
                return fromVirtualWord(event.wVirtualKeyCode);
            }
        }
    }
}

} // namespace tui
} // namespace sokoban
