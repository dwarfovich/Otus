#pragma once

#include "command.h"
#include "command_identifier.h"
#include "loggable.h"

#include <iostream>

class Logger;

class BulkerCommand : public Command, public Loggable
{
public:
    BulkerCommand(const CommandIdentifier& identifier, std::ostream& stream = std::cout, const std::shared_ptr<Logger>& logger = nullptr)
        : id_ { identifier }, stream_{stream}, Loggable { logger }
    {
    }

    void execute() override
    {
        stream_ << id_.identifier() << '\n';
        if (logger_) {
            log(id_.identifier());
        }
    }

private:
    CommandIdentifier id_;
    std::ostream&     stream_;
};