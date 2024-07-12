#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <limits>

#include "ip.h"

// Vector of IPs and flag, that indicates some errors occured.
std::pair<std::vector<ip::Ip>, bool> readIps(std::istream& stream);

int main(int argc, char* argv[])
{
#ifdef _DEBUG
    std::ifstream stream { "../../../ip_filter_test1.tsv" };
    if (!stream.is_open()) {
        std::cerr << "Failed to open input stream\n";
        return 1;
    }
#else
    std::istream& stream = std::cin;
#endif // _DEBUG

#ifdef _DEBUG
    std::ofstream outStream { "../../../result.tsv" };
    if (!outStream.is_open()) {
        std::cerr << "Failed to open output stream\n";
        return 1;
    }
#else
    std::ostream& outStream = std::cout;
#endif // _DEBUG

    auto [ips, success] = readIps(stream);
    if (!success) {
        std::cerr << "Couldn't parse some IPs. Quiting." << std::endl;
        return 1;
    }

    using IpsIterator = decltype(ips)::iterator;
    std::vector<std::function<void(IpsIterator begin, IpsIterator end)>> jobs;
    jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
        if (begin == end) {
            return;
        }
        std::sort(begin, end, std::greater());
        outStream << *begin;
        std::for_each(begin + 1, end, [&outStream](const auto& ip) {
            outStream << '\n' << ip;
        });
    });
    jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
        std::for_each(begin, end, [&outStream](const auto& ip) {
            if (ip[0] == 1) {
                outStream << '\n' << ip;
            }
        });
    });
    jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
        std::for_each(begin, end, [&outStream](const auto& ip) {
            if (ip[0] == 46 && ip[1] == 70) {
                outStream << '\n' << ip;
            }
        });
    });

    jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
        std::for_each(begin, end, [&outStream](const auto& ip) {
            if (ip[0] == 46 || ip[1] == 46 || ip[2] == 46 || ip[3] == 46) {
                outStream << '\n' << ip;
            }
        });
    });

    for (const auto& job : jobs) {
        job(ips.begin(), ips.end());
    }

    outStream << std::endl;

    return 0;
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