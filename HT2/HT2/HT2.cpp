#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <limits>

namespace ip {

constexpr inline size_t partsCount = 4;
constexpr inline char   delimiter  = '.';

class Ip
{
public:
    friend std::ostream &operator<<(std::ostream &input, const Ip &ip);
    friend std::istream &operator>>(std::istream &input, Ip &ip);

private:
    std::array<uint8_t, partsCount> values_ = {};
};

std::ostream &operator<<(std::ostream &output, const ip::Ip &ip)
{
    for (size_t i = 0; i < partsCount; ++i) {
        output << static_cast<uint16_t>(ip.values_[i]);
        if (i != partsCount - 1) {
            output << '.';
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
        if (i != 3) {
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

} // namespace ip

int main()
{
    std::stringstream ss { "1.2.3.4 23 wd\n11a.22.33.44 43 452" };
    ip::Ip            ip;
    std::string       skip;
    while (ss >> ip) {
        std::getline(ss, skip);
        std::cout << ip << '\n';
    }

    return 0;
}
