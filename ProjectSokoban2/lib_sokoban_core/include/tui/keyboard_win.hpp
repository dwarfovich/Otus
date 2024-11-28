#pragma once

#include "system_init.hpp"

#include <windows.h>

namespace sokoban {
namespace tui {

char waitForInput()
{
    static INPUT_RECORD inputRecord;
    static DWORD        eventsRead = 0;
    while(true) {
        auto success = ReadConsoleInput(System::inputHandle(), &inputRecord, 1, &eventsRead);
        if (!success) {
            return 0;
        }
        if (inputRecord.EventType == KEY_EVENT){
            const auto& event = inputRecord.Event.KeyEvent;
            if(event.bKeyDown){
                return static_cast<char>(event.wVirtualKeyCode);
            }
        }
    }
}

} // namespace tui
} // namespace sokoban
