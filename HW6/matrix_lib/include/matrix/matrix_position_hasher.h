#pragma once

#include <cstddef>
#include <vector>
#include <unordered_set>

struct MatrixPositionHasher
{
    constexpr std::size_t operator()(const std::vector<std::size_t>& vector) const {
        std::hash<std::size_t> elementHasher;
        std::size_t hash = 0;
        for(std::size_t i = 0; i < vector.size(); ++i){
            hash = (hash << 1) ^ elementHasher(vector[i]);
        }
        return 0;
    }
};