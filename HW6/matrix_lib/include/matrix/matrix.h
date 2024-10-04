#pragma once

#include "matrix_position_hasher.h"

#include <cstddef>
#include <unordered_map>
#include <array>
#include <stdexcept>

inline constexpr std::size_t invalidOrdinal = static_cast<std::size_t>(-1);

template<typename ElementType, std::size_t Dimension = 2, typename Hasher = MatrixPositionHasher<Dimension>>
    requires(Dimension != 0 && Dimension != invalidOrdinal)
class SparseMatrix
{
public: // types
    class ElementProxy
    {
    public: // types
        using MatrixType = SparseMatrix<ElementType, Dimension, Hasher>;

    public: // methods
        ElementProxy(MatrixType& matrix, std::size_t firstIndex) : currentOrdinal_ { 1 }, matrix_ { matrix }
        {
            position_.back() = invalidOrdinal;
            position_.front() = firstIndex;
        }
        ElementProxy(MatrixType& matrix, const MatrixType::Position& position)
            : currentOrdinal_ { 1 }, matrix_ { matrix }, position_ { position }
        {
            currentOrdinal_ = Dimension;
        }
        ElementProxy(const ElementProxy& rhs)
            : currentOrdinal_ { invalidOrdinal }, position_ { rhs.position_ }, matrix_ { rhs.matrix_ }
        {
        }

        operator ElementType() { return matrix_.copyElement(position_); }

        ElementProxy& operator[](std::size_t index)
        {
            throwIfOrdinalIsOutOfRange(Dimension);
            position_[currentOrdinal_++] = index;

            return *this;
        }

        ElementProxy& operator=(const ElementType& element)
        {
            throwIfOrdinalIsOutOfRange(Dimension + 1);
            throwIfPositionIsInvalid();

            matrix_.set(position_, element);

            return *this;
        }

        ElementType  element() const noexcept { return matrix_.copyElement(position_); }
        ElementType& elementRef() const noexcept { return matrix_.trueRef(position_); }

    private: // methods
        void throwIfOrdinalIsOutOfRange(std::size_t criticalValue)
        {
            if (currentOrdinal_ >= criticalValue) {
                throw std::logic_error("It is forbidden to assign a value to copy-constructed proxy element and/or "
                                       "increment ordinal above Dimension.");
            }
        }
        void throwIfPositionIsInvalid()
        {
            if (position_.back() == invalidOrdinal) {
                throw std::logic_error("Position is invalid.");
            }
        }

    private: // data
        size_t                                     currentOrdinal_ = 0;
        MatrixType&                                matrix_;
        mutable std::array<std::size_t, Dimension> position_ = {};
    };

    using Position = std::array<std::size_t, Dimension>;

    class Iterator 
    {
    public: // types
        using MatrixType = SparseMatrix<ElementType, Dimension, Hasher>;

    public: // methods
        Iterator() = default;
        Iterator(const MatrixType::ElementsContainer::iterator& iterator) : iterator_ { iterator } {}

        bool operator==(const Iterator& rhs) const noexcept{
            return iterator_ == rhs.iterator_;
        }

        Iterator& operator++(){ 
            ++iterator_;

            return *this;
        }
        const ElementType& operator*() const {
            return iterator_->second;
        }

        const ElementType& operator*() { return iterator_->second; }

    private:
        MatrixType::ElementsContainer::iterator iterator_;
    };

    

public: // methods
    SparseMatrix(const ElementType& defaultElement = -1) : defaultElement_ { defaultElement }
    {
    }

    ElementProxy operator[](std::size_t index) { return { *this, index }; }

    Iterator begin() const noexcept {
        auto& t = const_cast<SparseMatrix*>(this)->elements_;
        auto  tt = const_cast<SparseMatrix*>(this)->removeConstness(t,
                                  elements_.begin());
        return {tt};
    }
    Iterator end() const noexcept { 
        auto& t  = const_cast<SparseMatrix*>(this)->elements_;
        auto  tt = const_cast<SparseMatrix*>(this)->removeConstness(t, elements_.end());
        return { tt };
    }

    std::size_t        size() const noexcept { return elements_.size(); }
    bool               empty() const noexcept { return size() == 0; }
    const ElementType& defaultElement() const noexcept { return defaultElement_; }
    // ElementProxy       proxyAt(const Position& position) const noexcept;
    void set(const Position& position, const ElementType& element) { elements_[position] = element; }
    void set(const Position& position, ElementType&& element) { elements_[position] = element; }

    ElementType& trueRef(const Position& position) const noexcept
    {
        auto iter = elements_.find(position);
        if (iter == elements_.cend()) {
            return defaultElement_;
        } else {
            return iter->second;
        }
    }
    ElementType copyElement(const Position& position) const noexcept { return trueRef(position); }

    private:
    /*template<typename Container, typename ConstIterator>
    typename Container::iterator remove_constness(Container& c, ConstIterator it)
    {
        return c.erase(it, it);
    }*/
        
        template<typename Container, typename ConstIterator>
        typename Container::iterator removeConstness(Container& c, ConstIterator it)
        {
            return c.erase(it, it);
        }

    private:
    using ElementsContainer = std::unordered_map<Position, ElementType, Hasher>;

    ElementsContainer elements_;
    ElementType       defaultElement_;
};

// template<typename ElementType, std::size_t Dimension, typename Hasher>
// auto SparseMatrix<ElementType, Dimension, Hasher>::proxyAt(const Position& position) const noexcept -> ElementProxy
//{
//     ElementProxy element { *this };
//     auto         iter = elements_.find(position);
//     if (iter == elements_.cend()) {
//         return { *this, &defaultElement_ };
//     } else {
//         return { *this, &iter->second };
//     }
// }

//std::unordered_map<std::array<std::size_t, >, ElementType, Hasher>::iterator removeIteratorConstness(
//    std::unordered_map<Position, ElementType, Hasher>::const_iterator it) const
//{
//    return elements_.erase(it, it);
//}