#pragma once

#include <cinttypes>

namespace sokoban{

class Coords{
public:
    Coords() = default;
    Coords(std::size_t x, std::size_t y) : x_{x},y_{y}{} 

    std::size_t x() const {
        return x_;
    }
    std::size_t y() const { return y_; }
    private:
        std::size_t x_ = 0;
        std::size_t y_ = 0;
};

}