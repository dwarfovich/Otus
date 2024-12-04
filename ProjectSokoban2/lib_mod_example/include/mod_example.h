#pragma once

#include "sokoban_core/mod.h"
#include "sokoban_core/session_context.hpp"

class ModExample : public sokoban::Mod
{
public:
    const std::string& name() const override
    {
        static const std::string name = "ModExample";
        return name;
    }

    std::unique_ptr<sokoban::SessionContext> createSessionContext() const override { return nullptr; }
};
