#pragma once

#include <cinttypes>
#include <limits>
#include <iostream>
#include <cmath>

bool canSafelyIncrementDouble(double value)
{
    return value + 1.0 < std::numeric_limits<double>::max();
}

bool calculateDispersion(double valueNumber, std::size_t sum, std::size_t sumSquared, double& dispersion)
{
    const double dividedSum = sum / valueNumber;
    if (!std::isfinite(dividedSum)) {
        return false;
    }
    
    dispersion = sumSquared / valueNumber - dividedSum * dividedSum;
    
    return std::isfinite(dispersion);
}