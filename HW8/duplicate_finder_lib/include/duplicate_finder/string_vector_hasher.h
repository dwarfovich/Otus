#pragma once

#include "string_vector.h"

#include <cinttypes>

struct StringVectorHasher
{
    std::size_t operator()(const StringVector& sv) const{
        return 0;
    }
};