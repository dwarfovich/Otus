#pragma once

#include <cstddef>
#include <unordered_map>
#include <array>

// template<typename ElementType>
// class ElementProxyRef
//{
//     template<typename ElementType, std::size_t Dimension, ElementType DefaultElement>
//     friend class Matrix;
//
// public:
//
// private:
//     ElementType* element_ = nullptr;
//     Matrix<ElementType>& matrix_;
// };
//
// template<typename ElementType, std::size_t Dimension = 2>
// class ElementProxy
//{
//     template<typename ElementType, std::size_t Dimension, ElementType DefaultElement>
//     friend class Matrix;
//
// public:
// private:
//     ElementType element_;
// };

template<typename ElementType, std::size_t Dimension = 2, ElementType DefaultElement = ElementType()>
class Matrix
{
public:
    class ElementProxyRef
    {
    public:
        ElementProxyRef(Matrix<ElementType, Dimension, DefaultElement>& matrix) : matrix_{matrix}{}
    private:
        ElementType*                                    element_ = nullptr;
        Matrix<ElementType, Dimension, DefaultElement>& matrix_;
    };

    using Position = std::array<std::size_t, Dimension>;

    ElementProxyRef refAt(const Position& position) const noexcept;

private:
    std::unordered_map<Position, ElementType> elements_;
};

template<typename ElementType, std::size_t Dimension, ElementType DefaultElement>
auto Matrix<ElementType, Dimension, DefaultElement>::refAt(const Position& position) const noexcept -> ElementProxyRef
{
    auto iter = elements_.find(position);
    if (iter == elements_.cend()) {
        //cachedElement_.element_ = &DefaultElement;
    } else {
        //cachedElement_.element = &iter->second;
    }

    return {};
}
