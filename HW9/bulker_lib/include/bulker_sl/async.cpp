#include "async.h"
#include "command_executor_context.h"
#include "command_executor_context_hasher.h"

#include <memory>
#include <unordered_set>

namespace async {

class ContextDispatcher
{
public:
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
        contexts_.erase((CommandExecutorContext*)handle);
    }

private:
    std::unordered_set<CommandExecutorContext*> contexts_;
    std::mutex                                  mutex_;
};

static ContextDispatcher contextDispatcher;

static std::mutex globalMutex;

handle_t connect(std::size_t bulk)
{
    std::lock_guard lock { globalMutex };
    return contextDispatcher.createContext(bulk);
}

void receive(handle_t handle, const char* data, std::size_t size)
{
    std::lock_guard lock { globalMutex };
    auto c = contextDispatcher.getContext(handle);
    if (!c) {
        return;
    }

    c->readCommands(data, size);
}

void disconnect(handle_t handle)
{
    std::lock_guard lock { globalMutex };
    auto c = contextDispatcher.getContext(handle);
    if (!c) {
        return;
    }
}

} // namespace async
