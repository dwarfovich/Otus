#pragma once

#include "array_hasher.h"

#include <cstddef>
#include <unordered_map>
#include <array>

template<typename ElementType, std::size_t Dimension = 2, typename Hasher = ArrayHasher<ElementType, Dimension>>
class Matrix
{
public:
    class ElementProxyRef
    {
    public:
        using MatrixType = Matrix<ElementType, Dimension>;

        ElementProxyRef(MatrixType& matrix, ElementType* element = nullptr) : matrix_ { matrix }, element_ { element }
        {
        }

    private:
        ElementType* element_ = nullptr;
        MatrixType&  matrix_;
    };

    using Position = std::array<std::size_t, Dimension>;

    Matrix(const ElementType& defaultElement = {}) : defaultElement_ { defaultElement } {}

    const ElementType& defaultElement() const noexcept { return defaultElement_; }
    ElementProxyRef    refAt(const Position& position) const noexcept;
    void               set(const Position& position, const ElementType& element) { elements_[position] = element; }
    void               set(const Position& position, ElementType&& element) { elements_[position] = element; }

private:
    using ElementsContainer = std::unordered_map<Position, ElementType, Hasher>;
    ElementsContainer elements_;
    ElementType       defaultElement_;
};

template<typename ElementType, std::size_t Dimension, typename Hasher>
auto Matrix<ElementType, Dimension, Hasher>::refAt(const Position& position) const noexcept -> ElementProxyRef
{
    ElementProxyRef element { *this };
    auto            iter = elements_.find(position);
    if (iter == elements_.cend()) {
        return { *this, &defaultElement_ };
    } else {
        return { *this, &iter->second };
    }
}
