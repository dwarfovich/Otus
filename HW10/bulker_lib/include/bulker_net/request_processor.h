#pragma once

// #include "bulker_sl/command_executor_context.h"
#include "bulker_sl/async.h"

#include <string>
#include <memory>
#include <optional>

class RequestProcessor
{
public:
    RequestProcessor(std::size_t staticBlockSize) : staticBlockSize_ { staticBlockSize } {}

    async::handle_t createContext(std::ostream& stream) { return async::connect(staticBlockSize_, stream,  contextDispatcher_); }
    void            removeContext(async::handle_t handle){ async::disconnect(handle, contextDispatcher_);
    }

    std::optional<std::string> process(async::handle_t handle, const std::string& requestString)
    {
        async::receive(handle, requestString.c_str(), 0, contextDispatcher_);

        return {};
    }

private: // methods
    

private: // data
    std::size_t              staticBlockSize_ = 1;
    mutable async::ContextDispatcher contextDispatcher_;
};