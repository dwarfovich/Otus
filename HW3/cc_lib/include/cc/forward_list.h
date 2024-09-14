#pragma once

#include <memory>
#include <type_traits>
#include <cinttypes>

#include <list>

namespace cc {

namespace details {

struct Node
{
    Node* nextNode = nullptr;
};

template<typename T>
struct DataNode : Node
{
    DataNode() = default;
    DataNode(const T& aData) : data { aData } {}
    DataNode(const T& aData, Node* nextNode) : Node { nextNode }, data { aData } {}

    T data;
};

} // namespace details

template<class ForwardList>
class ForwardListIterator
{
public: // types
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = typename ForwardList::value_type;
    using pointer = typename ForwardList::pointer;
using reference = typename ForwardList::reference;

public: // methods
    ForwardListIterator() = default;
    ForwardListIterator(const ForwardList* list);
    ForwardListIterator(const ForwardList* list, details::Node* node);
   //ForwardListIterator(ForwardList* list, details::Node* node);

    operator ForwardListIterator<const ForwardList>() const
    {
        return {list, node};
    }
    

    const ForwardList* getList() const{
        return list;
    }
    bool operator==(const ForwardListIterator<const ForwardList>& rhs) const
    {
        return node == rhs.getNode() && list == rhs.getList();
    }
    //bool operator==(const ForwardListIterator<ForwardList>& rhs) { return node == rhs.node && list == rhs.list; }

    //auto              operator<=>(const ForwardListIterator&) const = default;
    const value_type& operator*() const { return static_cast<details::DataNode<ForwardList::value_type>*>(node)->data; }
    value_type&       operator*() { return static_cast<details::DataNode<ForwardList::value_type>*>(node)->data; }
    ForwardListIterator& operator++();
    ForwardListIterator  operator++(int);

    details::Node* getNode() const noexcept { return node; }

private:
    details::Node*     node = nullptr;
    const ForwardList* list = nullptr;
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
    ForwardList();

    allocator_type get_allocator() const noexcept;
    iterator       before_begin() noexcept;
    const_iterator cbefore_begin() const noexcept;
    iterator       begin() noexcept;
    const_iterator cbegin() const noexcept;
    iterator       end() noexcept;
    const_iterator cend() const noexcept;
    bool           empty() const noexcept;
    std::size_t    size() noexcept;
    iterator       insert_after(iterator position, const value_type& value);
    iterator       erase_after(iterator position);

private: // data
    Allocator     allocator;
    details::Node head;
    std::size_t   listSize = 0;
};

//template<typename T, typename U>
//bool operator==(const ForwardListIterator<ForwardList<T>>& lhs, const ForwardListIterator<ForwardList<U>>& rhs)
//{
//    return lhs.node == rhs.node && lhs.list == rhs.list;
//}

//template<typename T, typename U>
//bool operator==(const ForwardListIterator<const ForwardList<T>>& lhs,
//                const ForwardListIterator<const ForwardList<U>>& rhs)
//{
//    return lhs.node == rhs.node && lhs.list == rhs.list;
//}

template<class ForwardList>
ForwardListIterator<ForwardList>& ForwardListIterator<ForwardList>::operator++()
{
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
ForwardListIterator<ForwardList>::ForwardListIterator(const ForwardList* aList, details::Node* aNode)
    : list { aList }, node { aNode }
{
}



template<typename T, typename Allocator>
ForwardList<T, Allocator>::ForwardList()
{
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::insert_after(iterator position, const value_type& value) -> iterator
{
    using namespace details;
    using NodeAllcoator         = typename std::allocator_traits<Allocator>::template rebind_alloc<DataNode<T>>;
    NodeAllcoator nodeAllocator = allocator;
    auto*         newNode       = nodeAllocator.allocate(1);

    using NodeAllocatorTraits = std::allocator_traits<NodeAllcoator>;
    NodeAllocatorTraits::construct(nodeAllocator, newNode, value);
    if (empty()) {
        head.nextNode = newNode;
    } else {
        newNode->nextNode            = position.getNode()->nextNode;
        position.getNode()->nextNode = newNode;
    }
    ++listSize;

    return { this, newNode };
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::erase_after(iterator position) -> iterator
{
    auto iterToErase             = std::next(position);
    auto nextIter                = std::next(iterToErase);
    using namespace details;
    using NodeAllcoator          = typename std::allocator_traits<Allocator>::template rebind_alloc<DataNode<T>>;
    using NodeAllocatorTraits    = std::allocator_traits<NodeAllcoator>;
    NodeAllcoator nodeAllocator  = allocator;
    auto*         addressToErase = static_cast<DataNode<T>*>(iterToErase.getNode());
    NodeAllocatorTraits::destroy(nodeAllocator, addressToErase);
    nodeAllocator.deallocate(addressToErase, 1);
    if (nextIter == end()) {
        position.getNode()->nextNode = nullptr;
    } else {
        position.getNode()->nextNode = nextIter.getNode();
    }
    --listSize;

    return { this, position.getNode()->nextNode };
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::before_begin() noexcept -> iterator
{
    return { this, &head };
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::cbefore_begin() const noexcept -> const_iterator
{
    return {};
//    return { this, &head };
    //return ForwardListIterator<decltype(this)>{ this, &head };
    //return { this, &head };
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::begin() noexcept -> iterator
{
    return std::next(before_begin());
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::cbegin() const noexcept -> const_iterator
{
    return std::next(cbefore_begin());
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::end() noexcept -> iterator
{
    return { this };
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::cend() const noexcept -> const_iterator
{
    return end();
}

template<typename T, typename Allocator>
bool ForwardList<T, Allocator>::empty() const noexcept
{
    return listSize == 0;
}

template<typename T, typename Allocator>
std::size_t ForwardList<T, Allocator>::size() noexcept
{
    return listSize;
}

template<typename T, typename Allocator>
auto ForwardList<T, Allocator>::get_allocator() const noexcept -> allocator_type
{
    return allocator;
}

} // namespace cc