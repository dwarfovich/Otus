#pragma once

#include "command_parser.h"
#include "command_block.h"
#include "command_identifier.h"

class BulkerCommandParser : public CommandParser
{
public:
    BulkerCommandParser(std::istream& stream) : stream_{stream}{}

    void readCommands(std::istream& stream) override {
        std::string id;
        stream >> id;
        readCommands_.addCommandIdentifier( std::move(id) );
        notifier_(readCommands_);
    }
    void setReadyNotifier(const ReadyNotifier& notifier) override { notifier_ = notifier; }

private:
    std::istream& stream_;
    CommandBlock  readCommands_;
    ReadyNotifier notifier_;
};