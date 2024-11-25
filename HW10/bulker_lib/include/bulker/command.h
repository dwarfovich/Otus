#pragma once

#include <iosfwd>

class Command
{
public:
    virtual void execute() = 0;
};