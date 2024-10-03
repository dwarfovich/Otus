#pragma once

#include <cstddef>
#include <array>

template<std::size_t Size>
struct MatrixPositionHasher
{
    constexpr std::size_t operator()(const std::array<std::size_t, Size>& array) const {
        std::hash<std::size_t> elementHasher;
        std::size_t hash = 0;
        for(std::size_t i = 0; i < Size; ++i){
            hash = (hash << 1) ^ elementHasher(array[i]);
        }
        return 0;
    }
};