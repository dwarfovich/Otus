#pragma once

#include "base_session_context.hpp"

#include "sokoban_core/mod.hpp"
#include "sokoban_core/session_context.hpp"

#include <boost/dll.hpp>

class BaseMod : public sokoban::Mod
{
public:
    static std::unique_ptr<sokoban::Mod> create() { return std::make_unique<BaseMod>(); }

    const std::string& name() const override
    {
        static const std::string name = "Base Game";
        return name;
    }

    std::unique_ptr<sokoban::SessionContext> createSessionContext() const override
    {
        return std::make_unique<sokoban::sbg::BaseSessionContext>();
    }
};

BOOST_DLL_ALIAS(BaseMod::create, createMod)