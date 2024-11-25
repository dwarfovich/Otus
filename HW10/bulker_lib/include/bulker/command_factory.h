#pragma once

#include <memory>

class Command;
class CommandIdentifier;

class CommandFactory
{
public:
    virtual std::unique_ptr<Command> create(const CommandIdentifier& id) const = 0;

};