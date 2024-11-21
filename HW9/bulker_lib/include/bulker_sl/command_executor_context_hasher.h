#pragma once

#include "command_executor_context.h"

#include <cstddef>
#include <memory>

struct Compare
{
    size_t operator()(std::shared_ptr<CommandExecutorContext> const& a,
                      std::shared_ptr<CommandExecutorContext> const& b) const
    {
        return a.get() == b.get();
    }

    size_t operator()(std::shared_ptr<CommandExecutorContext> const& a,
                      const CommandExecutorContext* const b) const
    {
        return a.get() == b;
    }

    size_t operator()(const CommandExecutorContext* const a,
                      std::shared_ptr<CommandExecutorContext> const& b) const
    {
        return a == b.get();
    }
};

struct CommandExecutorContextHasher
{
    using transparent_key_equal = Compare;

    std::size_t operator()(const std::shared_ptr<CommandExecutorContext>& sptr) const { 
        return std::hash<CommandExecutorContext*>()(sptr.get());
         }

    std::size_t operator()(const CommandExecutorContext* ptr) const {
        return std::hash<const CommandExecutorContext*>()(ptr);
    }
};