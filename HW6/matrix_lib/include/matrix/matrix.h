#pragma once

#include <cstddef>
#include <unordered_map>
#include <array>

template<typename ElementType, std::size_t Dimension = 2, ElementType DeafaultElement = ElementType()>
class Matrix
{
public:
private:
    std::unordered_map<std::array<std::size_t, Dimension> , ElementType> elements_;
};