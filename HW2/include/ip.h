#pragma once

#include <iosfwd>
#include <array>
#include <tuple>
#include <cinttypes>

namespace ip {

constexpr inline size_t partsCount = 4;
constexpr inline char   delimiter  = '.';

class Ip
{
public:
    friend std::ostream &operator<<(std::ostream &input, const Ip &ip);
    friend std::istream &operator>>(std::istream &input, Ip &ip);

    constexpr uint8_t       &operator[](size_t i) { return values_[i]; }
    constexpr const uint8_t &operator[](size_t i) const { return values_[i]; }
    constexpr bool           operator==(const Ip &rhs) const { return values_ == rhs.values_; }
    constexpr bool           operator!=(const Ip &rhs) const { return values_ != rhs.values_; }
    constexpr bool           operator<(const Ip &rhs) const { return values_ < rhs.values_; }
    constexpr bool           operator<=(const Ip &rhs) const { return values_ <= rhs.values_; }
    constexpr bool           operator>(const Ip &rhs) const { return values_ > rhs.values_; }
    constexpr bool           operator>=(const Ip &rhs) const { return values_ >= rhs.values_; }

    constexpr bool is(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) const { 
        return p1 == values_[0] && p2 == values_[1] && p3 == values_[2] && p4 == values_[3];
    }

private:
    std::array<uint8_t, partsCount> values_ = {};
};

} // namespace ip