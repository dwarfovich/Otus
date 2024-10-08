#pragma once

#include "command_factory.h"
#include "bulker_command.h"

class BulkerCommandFactory : public CommandFactory
{
public:
    BulkerCommandFactory(const std::shared_ptr<Logger>& logger) : logger_ { logger } {}

    std::unique_ptr<Command> create(const CommandIdentifier& id) const override
    {
        return std::make_unique<BulkerCommand>(id, logger_);
    }

private:
    std::shared_ptr<Logger> logger_ = nullptr;
};