#pragma once

#include "widget.h"
#include "command.h"

#include <memory>

class Command;

class Controller : public Widget
{
public:
    Controller(const std::shared_ptr<Command>& command = nullptr): command_{command}{}
    virtual void activate() {
        command_->execute();
    }
    void setCommand(const std::shared_ptr<Command>& command) {
        command_ = command;
    }

private:
    std::shared_ptr<Command> command_;
};