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
            input.setstate(std::ios_base::failbit);
            return input;
        }
        if (i != partsCount - 1) {
            [[likely]] if (input.peek() == delimiter) {
                input.ignore();
            } else {
                input.setstate(std::ios_base::failbit);
                return input;
            }
        }
    }

    return input;
}

}