#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <limits>

#include "ip_lib/ip.h"

int main(int argc, char* argv[])
{
    std::istream& stream    = std::cin;
    std::ostream& outStream = std::cout;

    auto [ips, success] = ip::readIps(stream);
    if (!success) {
        std::cerr << "Couldn't parse some IPs. Quiting." << std::endl;
        return 1;
    }

    std::sort(ips.begin(), ips.end(), std::greater());
    for (const auto& ip : ips) {
        outStream << ip << '\n';
    }
    ip::filterPart0Is1(ips.begin(), ips.end(), std::cout);
    ip::filterPart0Is46AndPart1Is70(ips.begin(), ips.end(), std::cout);
    ip::filterAnyPartIs46(ips.begin(), ips.end(), std::cout);

    return 0;
}
