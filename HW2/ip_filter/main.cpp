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
    // std::istream& stream = std::cin;
    // std::ostream& outStream = std::cout;

    // auto [ips, success] = ip::readIps(stream);
    // if (!success) {
    //     std::cerr << "Couldn't parse some IPs. Quiting." << std::endl;
    //     return 1;
    // }

    // using IpsIterator = decltype(ips)::iterator;
    // std::vector<std::function<void(IpsIterator begin, IpsIterator end)>> jobs;
    // jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
    //     if (begin == end) {
    //         return;
    //     }
    //     std::sort(begin, end, std::greater());
    //     outStream << *begin;
    //     std::for_each(begin + 1, end, [&outStream](const auto& ip) {
    //         outStream << '\n' << ip;
    //     });
    // });
    // jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
    //     std::for_each(begin, end, [&outStream](const auto& ip) {
    //         if (ip[0] == 1) {
    //             outStream << '\n' << ip;
    //         }
    //     });
    // });
    // jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
    //     std::for_each(begin, end, [&outStream](const auto& ip) {
    //         if (ip[0] == 46 && ip[1] == 70) {
    //             outStream << '\n' << ip;
    //         }
    //     });
    // });

    // jobs.push_back([&outStream](IpsIterator begin, IpsIterator end) {
    //     std::for_each(begin, end, [&outStream](const auto& ip) {
    //         if (ip[0] == 46 || ip[1] == 46 || ip[2] == 46 || ip[3] == 46) {
    //             outStream << '\n' << ip;
    //         }
    //     });
    // });

    // for (const auto& job : jobs) {
    //     job(ips.begin(), ips.end());
    // }

    // outStream << std::endl;

    return 0;
}

