#pragma once

#include "mod_context.hpp"
#include "sokoban_core/mod.hpp"
#include "sokoban_core/session_context.hpp"

#include <boost/dll.hpp>

class ModExample : public sokoban::Mod
{
public:
    static std::unique_ptr<sokoban::Mod> create() { return std::make_unique<ModExample>(); }

    const std::string& name() const override
    {
        static const std::string name = "ModExample";
        return name;
    }

    std::unique_ptr<sokoban::SessionContext> createSessionContext() const override { return std::make_unique<ModSessionContext>(); }
};

BOOST_DLL_ALIAS(ModExample::create, createPlugin)