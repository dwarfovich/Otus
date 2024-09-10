#pragma once

#include <memory>
#include <cinttypes>

#include <list>

namespace cc {

template<typename T, typename Allocator = std::allocator<T>>
class ForwardList
{


public: // types
    template<typename T>
    class ForwardListIterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type      = T;

        const T& operator*() const {
            return *this;
        }

        T& operator*()  { return node->data; }

        ForwardListIterator& operator++();

        ForwardListIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const ForwardListIterator&) const {
            return false;
        }
    private:
        ForwardList<T, Allocator>::Node* node = nullptr;
        ForwardList<T, Allocator>*       list = nullptr;
    };

    using value_type      = T;
    using allocator_type  = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = std::allocator_traits<Allocator>::const_pointer;
    using iterator        = ForwardListIterator<T>;
    using const_iterator  = ForwardListIterator<const T>;

public:  // methods
    void f(){ 
        ForwardListIterator i;
//        i.node;
        ForwardList<int>::const_iterator iter;
        //++iter;

        std::list<int>::const_iterator iii;
     ++iii;
        *iii = 23;
    }
private: // types
    struct Node
    {
        template<typename U>
        friend class ForwardListIterator;
        T    data;
        Node nextNode = nullptr;
    };

private: // data
    Allocator   allocator;
    Node*       firstNode = nullptr;
    std::size_t size      = 0;
};

} // namespace cc