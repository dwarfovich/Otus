#include "async.h"
#include "command_executor_context.h"
#include "command_executor_context_hasher.h"

#include <memory>
#include <unordered_set>
#include <thread>
#include <chrono>

namespace async {

static std::mutex globalMutex;

handle_t connect(std::size_t bulk, ContextDispatcher& contextDispatcher)
{
    std::lock_guard lock { globalMutex };
    return contextDispatcher.createContext(bulk);
}

void receive(handle_t handle, const char* data, std::size_t /*reserved*/, ContextDispatcher& contextDispatcher)
{
    std::lock_guard lock { globalMutex };
    auto            c = contextDispatcher.getContext(handle);
    if (!c) {
        return;
    }

    c->readCommands(data);
}

void disconnect(handle_t handle, ContextDispatcher& contextDispatcher)
{
    std::lock_guard lock { globalMutex };
    contextDispatcher.closeContext(handle);
}

} // namespace async
