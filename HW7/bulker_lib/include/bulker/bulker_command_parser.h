#pragma once

#include "command_parser.h"
#include "command_block.h"
#include "command_identifier.h"

#include <iostream>

class BulkerCommandParser : public CommandParser
{
public:
    BulkerCommandParser(std::istream& stream, std::size_t staticBlockSize = 3)
        : stream_ { stream }, staticBlockSize_ { staticBlockSize }
    {
    }

    void readCommands(std::istream& stream) override {
        std::string id;
        while (std::getline(stream, id)) {
            readCommands_.addCommandIdentifier( std::move(id) );
            notifier_(readCommands_);
            id.clear();
            readCommands_.clear();
        }
    }
    void setReadyNotifier(const ReadyNotifier& notifier) override { notifier_ = notifier; }

private:
    std::istream& stream_;
    CommandBlock  readCommands_;
    ReadyNotifier notifier_;
    std::size_t staticBlockSize_ = 3;
};