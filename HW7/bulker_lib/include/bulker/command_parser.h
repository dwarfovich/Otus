#pragma once

#include <vector>
#include <functional>

namespace std {
class istream;
}

class CommandBlock;

class CommandParser
{
public: //  types
    using ReadyNotifier = std::function<void(const CommandBlock&)>;

public: // methods
    virtual void readCommands(std::istream& stream) = 0;
    virtual void setReadyNotifier(const ReadyNotifier& notifier) = 0;
};