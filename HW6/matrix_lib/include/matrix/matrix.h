#pragma once

#include "matrix_position_hasher.h"

#include <cstddef>
#include <unordered_map>
#include <array>

template<typename ElementType, std::size_t Dimension, typename Hasher>
class Matrix;

//template<template<class, class> class V, class T, class A>
//void f(V<T, A>& v)
//{
//    // This can be "typename V<T, A>::value_type",
//    // but we are pretending we don't have it
//
//    T temp = v.back();
//    v.pop_back();
//    // Do some work on temp
//
//    std::cout << temp << std::endl;
//}

template<typename ElementType, std::size_t Dimension, typename Hasher>
struct IBase
{
    IBase<ElementType, Dimension, Hasher>& operator[](std::size_t index)
    {
        position_[posIndex_] = index;
        ++posIndex_;
        return *this;
    }

    IBase<ElementType, Dimension, Hasher>& operator=(const ElementType& element)
    {
        matrix_.set(position_, element);

        return *this;
        //return *this;
    }

    size_t posIndex_ = 0;
    mutable std::array<std::size_t, Dimension> position_;
    Matrix<ElementType, Dimension, Hasher>&    matrix_;
};

template<typename ElementType, std::size_t Dimension = 2, typename Hasher = MatrixPositionHasher<Dimension>>
class Matrix
{
public:
    IBase<ElementType, Dimension, Hasher> operator[](std::size_t index) { return {0ul, {}, *this}; }

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
    void               set(const Position& position, const ElementType& element) {
        elements_[position] = element;
    }
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
