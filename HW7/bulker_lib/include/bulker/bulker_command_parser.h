#pragma once

#include "command_parser.h"
#include "command_block.h"

class BulkerCommandParser : public CommandParser
{
public:
    void readCommands(std::istream& stream) override {}
    void setReadyNotifier(const ReadyNotifier& notifier) override { notifier_ = notifier; }

private:
    CommandBlock  readCommands_;
    ReadyNotifier notifier_;
};