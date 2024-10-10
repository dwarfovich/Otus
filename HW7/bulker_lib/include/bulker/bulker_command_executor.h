#pragma once

#include "command_block.h"
#include "bulker_command_factory.h"

#include <memory>

class BulkerCommandExecutor
{
public:
    BulkerCommandExecutor(std::ostream& stream, const std::shared_ptr<Logger>& logger = nullptr) : factory_ {stream, logger }, stream_{stream} {
    }
    
    void onGotCommandBlock(const CommandBlock& block) {
        stream_ << "Bulk: ";
        for (std::size_t i = 0; i < block.identifiers().size(); ++i){
            auto command = factory_.create(block.identifiers()[i]);     
            command->execute();
            if (i != block.identifiers().size() - 1) {
                stream_ << ", ";
            }
        }
        stream_ << '\n';
    }

    private:
        BulkerCommandFactory factory_;
        std::ostream& stream_;
};