#pragma once

#include <cstddef>
#include "bulker_sl/command_executor_context.h"
#include <unordered_set>

#ifdef DLL_EXPORT
    #define PROJECT_API __declspec(dllexport)
#else
    #define PROJECT_API __declspec(dllimport)
#endif

namespace async {

using handle_t = void*;

class PROJECT_API ContextDispatcher
{
    friend PROJECT_API handle_t connect(std::size_t bulk, ContextDispatcher& dispatcher);
    friend PROJECT_API void receive(handle_t handle, const char* data, std::size_t size, ContextDispatcher& dispatcher);
    friend PROJECT_API void disconnect(handle_t handle, ContextDispatcher& dispatcher);

public:
    ContextDispatcher() = default;
    ~ContextDispatcher()
    {
        std::lock_guard lock { mutex_ };
        for (auto* c : contexts_) {
            delete c;
        }
    }

private:
    handle_t createContext(std::size_t staticBlockSize)
    {
        std::lock_guard lock { mutex_ };
        auto            context              = new CommandExecutorContext(staticBlockSize);
        const auto [iter, insertionHappened] = contexts_.insert(std::move(context));
        if (insertionHappened) {
            return context;
        } else {
            return nullptr;
        }
    }

    CommandExecutorContext* getContext(handle_t handle)
    {
        std::lock_guard lock { mutex_ };
        auto            iter = contexts_.find((CommandExecutorContext*)handle);
        if (iter == contexts_.cend()) {
            return nullptr;
        } else {
            return *iter;
        }
    }

    void closeContext(handle_t handle)
    {
        std::lock_guard lock { mutex_ };
        delete (CommandExecutorContext*)handle;
        contexts_.erase((CommandExecutorContext*)handle);
    }

private:
     std::unordered_set<CommandExecutorContext*> contexts_;
     std::mutex mutex_;
};

PROJECT_API handle_t connect(std::size_t bulk, ContextDispatcher& dispatcher);
PROJECT_API void     receive(handle_t handle, const char* data, std::size_t size, ContextDispatcher& dispatcher);
PROJECT_API void     disconnect(handle_t handle, ContextDispatcher& dispatcher);

} // namespace async
