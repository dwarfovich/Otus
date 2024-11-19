#pragma once

#include "command_factory.h"
#include "bulker_command.h"

class BulkerCommandFactory : public CommandFactory
{
public:
    BulkerCommandFactory(std::ostream& stream, const std::shared_ptr<Logger>& logger) : stream_{stream}, logger_ { logger } {}

    std::unique_ptr<Command> create(const CommandIdentifier& id) const override
    {
        return std::make_unique<BulkerCommand>(id,stream_, logger_);
    }

private:
    std::ostream& stream_;
    std::shared_ptr<Logger> logger_ = nullptr;
};