#pragma once

#include "sokoban_core/system.hpp"
#include "key_definitions_win.hpp"
#include "sokoban_core/windows.hpp"

namespace sokoban {
namespace tui {

//inline Key waitForInput()
//{
//    static INPUT_RECORD inputRecord;
//    static DWORD        eventsRead = 0;
//    while(true) {
//
//        auto success = ReadConsoleInput(System::inputHandle(), &inputRecord, 1, &eventsRead);
//        if (!success) {
//            return Key::invalidKey;
//        }
//        if (inputRecord.EventType == KEY_EVENT){
//            const auto& event = inputRecord.Event.KeyEvent;
//            if(event.bKeyDown){
//                return fromVirtualWord(event.wVirtualKeyCode);
//            }
//        }
//    }
//}

} // namespace tui
} // namespace sokoban
