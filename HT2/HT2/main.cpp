#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

#include "ip.h"

// Vector of IPs and flag, that indicates some errors occured.
std::pair<std::vector<ip::Ip>, bool> readIps(std::istream& stream);

int main(int argc, char* argv[])
{
#ifdef _DEBUG
    std::ifstream stream {"../../../ip_filter.tsv"};
#endif // _DEBUG

    auto [ips, success] = readIps(stream);
    if(!success){
        std::cerr << "Couldn't parse some IPs. Quiting."<< std::endl;
        return 1;
    }
    
    using IpsIterator = decltype(ips)::iterator;
    std::vector<std::function<void(IpsIterator begin, IpsIterator end)>> jobs;
    jobs.push_back([](IpsIterator begin, IpsIterator end){
        std::sort(begin, end, std::greater());
        });



    for(const auto& job : jobs){
        job(ips.begin(), ips.end());
    
    
    }
    for (const auto& ip : ips){
        std::cout << ip << std::endl;
    }

    return 0;
}

std::pair<std::vector<ip::Ip>, bool> readIps(std::istream& stream)
{
    std::pair<std::vector<ip::Ip>, bool> result;
    ip::Ip ip;
    while (stream >> ip) {
        result.first.push_back(std::move(ip));
        constexpr auto noLimit = std::numeric_limits<std::streamsize>::max();
        stream.ignore(noLimit, '\n');
    }
    result.second = stream.fail();

    return result;
}