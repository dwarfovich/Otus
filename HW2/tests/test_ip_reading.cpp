#include <sstream>
#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "ip_lib/ip.h"

TEST(IpTest, ValidReadIpsTest)
{
    auto check = [](std::vector<std::array<int, 4>> input) {
        std::stringstream ss;
        for(const auto& ip: input){
            ss<<ip[0] << '.' << ip[1] << '.' << ip[2] << '.' << ip[3] << '\n';
        }
        auto [ips, success] = ip::readIps(ss);
        
        ASSERT_TRUE(success);
        ASSERT_EQ(ips.size(), input.size());

    };

    check({});
    check({{}});
    check({{1,1,1,1}});
    check({{1,2,3,4}, {5,6,7,8}});
    check({{1,2,3,4}, {5,6,7,8}, {9,10,254,255}, {255,0,0,1}, {0,255,0,255}});
}

TEST(IpTest, InvalidReadIpsTest)
{
    auto check = [](std::string input, int errorCaseNumber = 0) {
        std::stringstream ss {input};
        auto [ips, success] = ip::readIps(ss);
        
        std::cout << ips.size() << " " << success << std::endl;
        ASSERT_FALSE(success);

    };

    check({{"1"}});
    check({{"1\n1.1.1.1"}});
    check({{"1.1.1.1\n1.1.1.\n1.1.1.1"}});
    // check({{"1.1.1.1\n1.1.1.2\n\n\n3333"}});
    // check({{"1.1.1.1\n1.1.1.2\n\n\na.2.3.4"}});
    // check({{"1.1.q.1\n1.1.1.2\n\n\n1.2.3.4"}});
}