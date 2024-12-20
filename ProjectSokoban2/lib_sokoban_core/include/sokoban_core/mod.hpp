#pragma once

#include "session_context.hpp"

#include <boost/config.hpp>

#include <string>

namespace sokoban {

class BOOST_SYMBOL_VISIBLE Mod
{
public:
    virtual ~Mod() = default;

    virtual const std::string&              name() const                 = 0;
    virtual std::unique_ptr<SessionContext> createSessionContext() const = 0;
};

} // namespace sokoban