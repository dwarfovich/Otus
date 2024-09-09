#pragma once

#include <memory>
#include <cinttypes>

namespace cc {

template<typename T, typename Allocator = std::allocator<T>>
class ForwardList
{
public:
private: // types
    struct Node
    {
        T                     data;
        std::unique_ptr<Node> nextNode = nullptr;
    };

private: // data
    Allocator allocator;
    std::unique_ptr<Node> firstNode = nullptr;
    std::size_t size = 0;
};

} // namespace cc