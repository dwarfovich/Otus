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

private:
    std::array<uint8_t, partsCount> values_ = {};
};

} // namespace ip