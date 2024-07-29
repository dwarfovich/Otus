#include "ip.h"

#include <iostream>
#include <limits>

namespace ip {

std::ostream &operator<<(std::ostream &output, const ip::Ip &ip)
{
    for (size_t i = 0; i < partsCount; ++i) {
        output << static_cast<uint16_t>(ip.values_[i]);
        if (i != partsCount - 1) {
            output << delimiter;
        }
    }

    return output;
}

std::istream &operator>>(std::istream &input, Ip &ip)
{
    uint16_t t;
    for (size_t i = 0; i < partsCount; ++i) {
        [[unlikely]] if ((input >> t) && t <= std::numeric_limits<uint8_t>::max()) {
            ip.values_[i] = static_cast<uint8_t>(t);
        } else {
            if (!input.eof()) {
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

std::pair<std::vector<ip::Ip>, bool> readIps(std::istream& stream)
{
    static constexpr auto                noLimit = std::numeric_limits<std::streamsize>::max();
    std::pair<std::vector<ip::Ip>, bool> result;
    ip::Ip                               ip;
    while (stream >> ip) {
        result.first.push_back(std::move(ip));
        stream.ignore(noLimit, '\n');
    }
    result.second = !stream.bad();

    return result;
}

} // namespace ip