#pragma once

#include <cstddef>
#include <array>

template <typename T, std::size_t Size>
struct ArrayHasher
{
    constexpr std::size_t operator()(const std::array<T, Size>& array) const {
        std::hash<T> elementHasher;
        std::size_t hash = 0;
        for(std::size_t i = 0; i < Size; ++i){
            hash = (hash << 1) ^ elementHasher(array[i]);
        }
        return 0;
    }
};