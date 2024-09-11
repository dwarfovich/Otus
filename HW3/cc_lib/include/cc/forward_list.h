#pragma once

#include <memory>
#include <cinttypes>

#include <list>

namespace cc {

template<class ForwardList>
class ForwardListIterator
{
public:
    using difference_type = std::ptrdiff_t;
    using value_type      = typename ForwardList::value_type;

    const value_type& operator*() const { return *this; }
    value_type& operator*() { return node->data; }
    ForwardListIterator& operator++() {
            node = node->nextNode;
    }
    ForwardListIterator operator++(int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    bool operator==(const ForwardListIterator&) const { return false; }

private:
    ForwardList::Node* node = nullptr;
    ForwardList*       list = nullptr;
};

template<typename T, typename Allocator = std::allocator<T>>
class ForwardList
{
    friend ForwardListIterator;
public: // types
    using value_type      = T;
    using allocator_type  = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = std::allocator_traits<Allocator>::const_pointer;
    using iterator        = ForwardListIterator<ForwardList<T, Allocator>>;
    using const_iterator  = ForwardListIterator<const ForwardList<T, Allocator>>;

public: // methods
    void f()
    {
        //ForwardListIterator i;
        //        i.node;
        ForwardList<int>::const_iterator iter;
        //++iter;

        std::list<int>::const_iterator iii;
        ++iii;
//        *iii = 23;
    }

private: // types
    struct Node
    {
        template<typename U>
        friend class ForwardListIterator;
        T    data;
        Node* nextNode = nullptr;
    };

private: // data
    Allocator   allocator;
    Node*       firstNode = nullptr;
    std::size_t size      = 0;
};

} // namespace cc