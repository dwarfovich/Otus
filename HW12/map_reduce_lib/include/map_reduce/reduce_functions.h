#pragma once

#include <cinttypes>
#include <limits>

bool canSafelyIncrementDouble(double value)
{
    return value + 1.0 < std::numeric_limits<double>::max();
}