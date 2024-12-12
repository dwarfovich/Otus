#pragma once

#include "mod_game.hpp"
#include "sokoban_core/session_context.hpp"
#include "sokoban_core/command.hpp"
#include "base_multimodal_interface.hpp"

#include <memory>

class ModSessionContext : public sokoban::SessionContext
{
public:
    sokoban::MultimodalInterface& multimodalInterface() { return multimodalInterface_; }
    std::pair<bool, bool>         executeCommand(const std::shared_ptr<sokoban::Command>& command) {
        return {};
    }
    void                          startGame() override {
        //sessionContext.drawLevel(sessionContext.game().map());
        /*bool finished = false;
        do {
            sokoban::Key c = sokoban::tui::waitForInput();
            if (c == sokoban::Key::esc) {
                return;
            }
            finished = sessionContext.executeCommand(std::make_shared<sokoban::Command>(c));
        } while (!finished);

        system("cls");
        std::cout << "Yoy won!!!\n";
        std::cout << "Press any key to return to main window\n";
        sokoban::tui::waitForInput();*/
    }
    sokoban::Game&                 game() override{
        return game_;
    }

    private: // methods
   /* void drawLevel(const BaseGame::RectangleTileMap& level){
    }*/
private: // data
    sokoban::sbg::BaseMultimodalInterface multimodalInterface_;
    ModGame game_;
};