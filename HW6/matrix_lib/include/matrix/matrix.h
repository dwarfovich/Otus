#pragma once

#include "matrix_position_hasher.h"

#include <cstddef>
#include <unordered_map>
#include <array>
#include <stdexcept>
#include <iostream>

template<typename ElementType,
         std::size_t Dimension      = 2,
         typename Hasher            = MatrixPositionHasher<Dimension>,
         std::size_t InvalidOrdinal = static_cast<std::size_t>(-1)>
    requires(Dimension != 0 && Dimension != InvalidOrdinal && Dimension < InvalidOrdinal)
class SparseMatrix
{
public: // types
    using Position = std::array<std::size_t, Dimension>;
    using Element  = ElementType;

    class ElementProxy
    {
    public: // types
        using MatrixType = SparseMatrix<Element, Dimension, Hasher>;

    public: // methods
        ElementProxy(MatrixType& matrix, std::size_t firstIndex) : currentOrdinal_ { 1 }, matrix_ { matrix }
        {
            position_.back()  = InvalidOrdinal;
            position_.front() = firstIndex;
        }
        ElementProxy(MatrixType& matrix, const MatrixType::Position& position)
            : currentOrdinal_ { 1 }, matrix_ { matrix }, position_ { position }
        {
            currentOrdinal_ = Dimension;
        }
        ElementProxy(const ElementProxy& rhs)
            : currentOrdinal_ { InvalidOrdinal }
            , position_ { rhs.hasValidPosition()
                              ? rhs.position_
                              : throw std::
                                  logic_error { "Trying to copy-construct from proxy with invalid Position." } }
            , matrix_ { rhs.matrix_ }
        {
        }

        operator ElementType() { return matrix_.copyElement(position_); }

        ElementProxy& operator[](std::size_t index)
        {
            throwIfOrdinalIsOutOfRange(Dimension);

            position_[currentOrdinal_++] = index;

            return *this;
        }

        ElementProxy& operator=(const Element& element)
        {
            throwIfOrdinalIsOutOfRange(Dimension + 1);
            throwIfPositionIsInvalid();

            matrix_.set(position_, element);

            return *this;
        }

        bool operator==(const Element& rhs) const noexcept { return rhs == matrix_.trueRef(position_); }

        std::ostream operator<<(std::ostream& stream) const
        {
            stream << elementRef();

            return stream;
        }

        Element  element() const noexcept { return matrix_.copyElement(position_); }
        Element& elementRef() const noexcept { return matrix_.trueRef(position_); }
        bool     hasValidPosition() const noexcept { return position_.back() != InvalidOrdinal; }

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
            if (!hasValidPosition()) {
                throw std::logic_error("Position is invalid.");
            }
        }

    private: // data
        size_t           currentOrdinal_ = 0;
        MatrixType&      matrix_;
        mutable Position position_ = {};
    };

    class Iterator
    {
    public: // types
        using MatrixType                   = SparseMatrix<Element, Dimension, Hasher>;
        using ElementData                  = std::pair<Position, Element>;
        using ElementDataWithConstPosition = std::pair<const Position, Element>;

    public: // methods
        Iterator() = default;
        Iterator(const MatrixType::ElementsContainer::iterator& iterator) : iterator_ { iterator } {}

        operator std::tuple<Position&, Element&>() { return { iterator_.first, iterator_.second }; }
        const ElementData&            operator*() const { return *iterator_; }
        ElementDataWithConstPosition& operator*() { return *iterator_; }
        bool      operator==(const Iterator& rhs) const noexcept { return iterator_ == rhs.iterator_; }
        Iterator& operator++()
        {
            ++iterator_;

            return *this;
        }

    private:
        MatrixType::ElementsContainer::iterator iterator_;
    };

public: // methods
    SparseMatrix(const Element& defaultElement = -1) : defaultElement_ { defaultElement } {}

    ElementProxy operator[](std::size_t index) { return { *this, index }; }

    Iterator begin() const noexcept
    {
        return { const_cast<SparseMatrix*>(this)->removeIteratorConstness(elements_.cbegin()) };
    }

    Iterator end() const noexcept
    {
        return { const_cast<SparseMatrix*>(this)->removeIteratorConstness(elements_.cend()) };
    }

    std::size_t    size() const noexcept { return elements_.size(); }
    bool           empty() const noexcept { return size() == 0; }
    const Element& defaultElement() const noexcept { return defaultElement_; }
    void           set(const Position& position, const Element& element)
    {
        if (element == defaultElement_) {
            elements_.erase(position);
        } else {
            elements_[position] = element;
        }
    }
    void set(const Position& position, Element&& element)
    {
        if (element == defaultElement_) {
            elements_.erase(position);
        } else {
            elements_[position] = element;
        }
    }

    const Element& trueRef(const Position& position) const noexcept
    {
        auto iter = elements_.find(position);
        if (iter == elements_.cend()) {
            return defaultElement_;
        } else {
            return iter->second;
        }
    }
    Element copyElement(const Position& position) const noexcept { return trueRef(position); }

private: // types
    using ElementsContainer = std::unordered_map<Position, Element, Hasher>;

private: // methods
    ElementsContainer::iterator removeIteratorConstness(ElementsContainer::const_iterator&& it)
    {
        return elements_.erase(it, it);
    }

private: // data
    ElementsContainer elements_;
    Element           defaultElement_ = -1;
};

template<std::size_t ArraySize>
void printPosition(std::ostream& stream, const std::array<std::size_t, ArraySize>& array)
{
    stream << '{';
    for (std::size_t i = 0; i < ArraySize - 1; ++i) {
        stream << array[i] << ", ";
    }
    stream << array.back() << '}';
}