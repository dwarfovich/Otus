#pragma once

#include "command.h"
#include "command_identifier.h"
#include "loggable.h"

class Logger;

class BulkerCommand : public Command, public Loggable
{
public:
    BulkerCommand(const CommandIdentifier& identifier, const std::shared_ptr<Logger>& logger = nullptr)
        : id_ { identifier }, Loggable { logger }
    {
    }

    void execute() override
    {
        if (logger_) {
            log(id_.identifier());
        }
    }

private:
    CommandIdentifier id_;
};