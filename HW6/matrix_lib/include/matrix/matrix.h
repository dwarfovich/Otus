#pragma once

#include "matrix_position_hasher.h"

#include <cstddef>
#include <unordered_map>
#include <array>
#include <stdexcept>

template<typename ElementType, std::size_t Dimension = 2, typename Hasher = MatrixPositionHasher<Dimension>>
class SparseMatrix
{
public: // types
    class ElementProxy
    {
    public: // types
        using MatrixType = SparseMatrix<ElementType, Dimension, Hasher>;

    public:
        ElementProxy(MatrixType& matrix) : currentOrdinal_ { 0 }, matrix_ { matrix } {
            position_.back() = invalidOrdinal_;
        }
        ElementProxy(const ElementProxy& rhs)
            : currentOrdinal_ { invalidOrdinal_ }, position_ { rhs.position_ }, matrix_ { rhs.matrix_ }
        {
        }

        operator ElementType() { return matrix_.copyElement(position_); }

        ElementProxy& operator[](std::size_t index)
        {
            throwIfOrdinalIsOutOfRange();
            position_[currentOrdinal_++] = index;

            return *this;
        }

        ElementProxy& operator=(const ElementType& element)
        {
            throwIfOrdinalIsOutOfRange();
            throwIfPositionIsInvalid();

            matrix_.set(position_, element);

            return *this;
        }

        ElementType  element() const noexcept { return matrix_.copyElement(position_); }
        ElementType& elementRef() const noexcept { return matrix_.trueRef(position_); }

    private: // methods
        void throwIfOrdinalIsOutOfRange()
        {
            if (currentOrdinal_ >= Dimension) {
                throw std::logic_error("It is forbidden to assign a value to copy-constructed proxy element and/or "
                                       "increment ordinal above Dimension.");
            }
        }
        void throwIfPositionIsInvalid() {
            if (position_.back() == invalidOrdinal_) {
                throw std::logic_error("Position is invalid.");
            }
        }

    private: // data
        size_t                                     currentOrdinal_ = 0;
        MatrixType&                                matrix_;
        mutable std::array<std::size_t, Dimension> position_ = {};
    };

    using Position = std::array<std::size_t, Dimension>;

public: // methods
    SparseMatrix(const ElementType& defaultElement = -1) : defaultElement_ { defaultElement } {}

    ElementProxy operator[](std::size_t index) { return { *this }; }

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

public: // data
    static constexpr std::size_t invalidOrdinal_ = -1;

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
