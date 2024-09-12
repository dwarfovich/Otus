#pragma once

#include <memory>
#include <type_traits>
#include <cinttypes>

#include <list>

namespace cc {

namespace details {

template<typename T>
struct Node
{
    T     data;
    Node* nextNode = nullptr;
};

} // namespace details

template<class ForwardList>
class ForwardListIterator
{
public: // types
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = typename ForwardList::value_type;

public: // methods
    ForwardListIterator() = default;
    ForwardListIterator(const ForwardList* list);
    ForwardListIterator(const ForwardList* list, details::Node<value_type>* node);

    bool                 operator==(const ForwardListIterator& rhs) const;
    auto                 operator<=>(const ForwardListIterator&) const = default;
    const value_type&    operator*() const { return *this; }
    value_type&          operator*() { return node->data; }
    ForwardListIterator& operator++();
    ForwardListIterator  operator++(int);

    details::Node<value_type>* getNode() const noexcept { return node; }

private:
    details::Node<value_type>* node = nullptr;
    const ForwardList*         list = nullptr;
};

template<typename T, typename Allocator = std::allocator<T>>
class ForwardList
{
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
    iterator begin() const;
    const_iterator cbegin() const;
    iterator end() const;
    const_iterator cend() const;
    iterator insert_after(iterator position, const value_type& value);
    bool     empty() const noexcept;

private: // data
    Allocator         allocator;
    details::Node<T>* firstNode = nullptr;
    std::size_t       size      = 0;
};

template<class ForwardList>
ForwardListIterator<ForwardList>& ForwardListIterator<ForwardList>::operator++()
{
    if (node)
        node = node->nextNode;
    return *this;
}

template<class ForwardList>
ForwardListIterator<ForwardList> ForwardListIterator<ForwardList>::operator++(int)
{
    auto temp = *this;
    ++*this;
    return temp;
}

template<class ForwardList>
ForwardListIterator<ForwardList>::ForwardListIterator(const ForwardList* aList) : list { aList }
{
}

template<class ForwardList>
ForwardListIterator<ForwardList>::ForwardListIterator(const ForwardList* aList, details::Node<value_type>* aNode)
    : list { aList }, node { aNode }
{
}

template<class ForwardList>
bool ForwardListIterator<ForwardList>::operator==(const ForwardListIterator& rhs) const
{
    return node == rhs.node && list == rhs.list;
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::insert_after(iterator position, const value_type& value) -> iterator
{
    auto next             = std::next(position);
    using AllocatorTraits = std::allocator_traits<Allocator>;

    using NodeAllcoator         = typename std::allocator_traits<Allocator>::template rebind_alloc<details::Node<T>>;
    NodeAllcoator nodeAllocator = allocator;
    auto*         newNode       = nodeAllocator.allocate(1);
    using NodeAllocatorTraits   = std::allocator_traits<decltype(nodeAllocator)>;
    NodeAllocatorTraits::construct(nodeAllocator, newNode, value, next.getNode());

    if(!firstNode){
        firstNode = newNode;
    } else{
        position.getNode()->nextNode = newNode;
    }
    ++size;

    return {this, newNode};
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::begin() const -> iterator
{
    return { this, firstNode };
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::cbegin() const -> const_iterator
{
    return this->begin();
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::end() const -> iterator
{
    return { this };
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::cend() const -> const_iterator
{
    return this->end();
}

template<typename T, typename Allocator>
bool ForwardList<T, Allocator>::empty() const noexcept
{
    return size == 0;
}

} // namespace cc