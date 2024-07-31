#pragma once

#include <iosfwd>
#include <array>
#include <vector>
#include <algorithm>
#include <cinttypes>

namespace ip {

constexpr inline size_t partsCount = 4;
constexpr inline char   delimiter  = '.';

class Ip
{
public:
    friend std::ostream &operator<<(std::ostream &input, const Ip &ip);
    friend std::istream &operator>>(std::istream &input, Ip &ip);

    Ip() = default;
    Ip(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);

    constexpr uint8_t       &operator[](size_t i) { return parts_[i]; }
    constexpr const uint8_t &operator[](size_t i) const { return parts_[i]; }
    bool                     operator==(const Ip &rhs) const { return parts_ == rhs.parts_; }
    bool                     operator!=(const Ip &rhs) const { return parts_ != rhs.parts_; }
    bool                     operator<(const Ip &rhs) const { return parts_ < rhs.parts_; }
    bool                     operator<=(const Ip &rhs) const { return parts_ <= rhs.parts_; }
    bool                     operator>(const Ip &rhs) const { return parts_ > rhs.parts_; }
    bool                     operator>=(const Ip &rhs) const { return parts_ >= rhs.parts_; }

    constexpr bool is(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) const
    {
        return p1 == parts_[0] && p2 == parts_[1] && p3 == parts_[2] && p4 == parts_[3];
    }

private:
    std::array<uint8_t, partsCount> parts_ = {};
};

// Returns vector of read IPs and flag, that is true if reading performed without errors.
std::pair<std::vector<ip::Ip>, bool> readIps(std::istream &stream);

using IpsConstIterator = std::vector<ip::Ip>::const_iterator;
void filterPart0Is1(IpsConstIterator begin, IpsConstIterator end, std::ostream& outStream);
void filterPart0Is46AndPart1Is70(IpsConstIterator begin, IpsConstIterator end, std::ostream& outStream);
void filterAnyPartIs46(IpsConstIterator begin, IpsConstIterator end, std::ostream& outStream);

} // namespace ip