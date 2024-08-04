#include "ip.h"

#include <iostream>
#include <limits>
#include <string>
#include <sstream>

namespace ip {

Ip::Ip(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) : parts_ { p1, p2, p3, p4 }
{
}

std::ostream &operator<<(std::ostream &output, const ip::Ip &ip)
{
    output << static_cast<uint16_t>(ip.parts_[0]);
    for (size_t i = 1; i < partsCount; ++i) {
        output << delimiter << static_cast<uint16_t>(ip.parts_[i]);
    }

    return output;
}

std::istream &operator>>(std::istream &input, Ip &ip)
{
    uint16_t t;
    input >> std::noskipws;
    for (size_t i = 0; i < partsCount; ++i) {
        [[likely]] if ((input >> t) && t <= std::numeric_limits<uint8_t>::max()) {
            ip.parts_[i] = static_cast<uint8_t>(t);
        } else {
            [[unlikely]] if (!input.eof()) {
                input.setstate(std::ios_base::badbit);
            }
            return input;
        }
        if (i != partsCount - 1) {
            [[likely]] if (input.peek() == delimiter) {
                input.ignore();
            } else {
                input.setstate(std::ios_base::badbit);
                return input;
            }
        }
    }

    return input;
}

std::pair<std::vector<ip::Ip>, bool> readIps(std::istream &stream)
{
    static constexpr auto                noLimit = std::numeric_limits<std::streamsize>::max();
    std::pair<std::vector<ip::Ip>, bool> result;
    ip::Ip                               ip;
    try {
        while (stream >> ip) {
            result.first.push_back(std::move(ip));
            stream.ignore(noLimit, '\n');
        }
    } catch (...) {
        result.second = true;
    }
    result.second = !stream.bad();

    return result;
}

void filterPart0Is1(IpsConstIterator begin, IpsConstIterator end, std::ostream &outStream)
{
    std::for_each(begin, end, [&outStream](const auto &ip) {
        if (ip[0] == 1) {
            outStream << ip << '\n';
        }
    });
}

void filterPart0Is46AndPart1Is70(IpsConstIterator begin, IpsConstIterator end, std::ostream &outStream)
{
    std::for_each(begin, end, [&outStream](const auto &ip) {
        if (ip[0] == 46 && ip[1] == 70) {
            outStream << ip << '\n';
        }
    });
}

void filterAnyPartIs46(IpsConstIterator begin, IpsConstIterator end, std::ostream &outStream)
{
    std::for_each(begin, end, [&outStream](const auto &ip) {
        if (ip[0] == 46 || ip[1] == 46 || ip[2] == 46 || ip[3] == 46) {
            outStream << ip << '\n';
        }
    });
}

} // namespace ip