#pragma once

#include "command_block.h"
#include "bulker_command_factory.h"

class BulkerCommandExecutor
{
public:
    BulkerCommandExecutor(const std::shared_ptr<Logger>& logger = nullptr) : factory_{logger}{
    }
    
    void onGotCommandBlock(const CommandBlock& block) {
        for (const auto& id : block.identifiers()) {
            auto command = factory_.create(id);     
            command->execute();
        }
    }

    private:
        BulkerCommandFactory factory_;
};