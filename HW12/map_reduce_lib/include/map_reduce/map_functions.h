#pragma once

#include <cinttypes>

inline bool addToVar(std::size_t value, std::size_t& var){
    if ((var + value) < var) {
        return false;
    } else {
        var += value;
        return true;
    }
}