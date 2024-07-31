#include <sstream>
#include <vector>
#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "ip_lib/ip.h"

std::string filter(std::string input, void (*f)(ip::IpsConstIterator, ip::IpsConstIterator, std::ostream&))
{
    std::stringstream is { input };
    auto [ips, success] = ip::readIps(is);
    if (!success) {
        return "error";
    }
    std::stringstream os;
    f(ips.cbegin(), ips.cend(), os);
    return os.str();
};

TEST(FilterTest, FilterPart0Is1Test)
{
    auto specialFilter = [](std::string input) -> std::string {
        return filter(input, ip::filterPart0Is1);
    };

    ASSERT_EQ(specialFilter(""), "");
    ASSERT_EQ(specialFilter("0.0.0.0"), "");
    ASSERT_EQ(specialFilter("0.0.0.0\n2.3.4.5\n6.7.8.9\n0.1.1.1"), "");
    ASSERT_EQ(specialFilter("1.2.3.4\n2.1.1.1"), "1.2.3.4\n");
    ASSERT_EQ(specialFilter("2.1.1.1\n1.2.3.4\n3.1.1.1"), "1.2.3.4\n");
    ASSERT_EQ(specialFilter("1.1.1.1\n1.2.3.4\n1.3.4.5"), "1.1.1.1\n1.2.3.4\n1.3.4.5\n");
}

TEST(FilterTest, FilterPart0Is46AndPart1Is70Test)
{
    auto specialFilter = [](std::string input) -> std::string {
        return filter(input, ip::filterPart0Is46AndPart1Is70);
    };

    ASSERT_EQ(specialFilter(""), "");
    ASSERT_EQ(specialFilter("0.0.0.0"), "");
    ASSERT_EQ(specialFilter("0.0.46.0\n2.3.70.5\n0.0.46.70\n0.1.70.46"), "");
    ASSERT_EQ(specialFilter("46.70.3.4\n2.1.1.1"), "46.70.3.4\n");
    ASSERT_EQ(specialFilter("70.46.1.1\n46.70.3.4\n70.46.1.1"), "46.70.3.4\n");
    ASSERT_EQ(specialFilter("46.70.1.1\n46.70.3.4\n46.70.4.5"), "46.70.1.1\n46.70.3.4\n46.70.4.5\n");
}

TEST(FilterTest, FilterAnyPartIs46Test)
{
    auto specialFilter = [](std::string input) -> std::string {
        return filter(input, ip::filterAnyPartIs46);
    };

    ASSERT_EQ(specialFilter(""), "");
    ASSERT_EQ(specialFilter("0.0.0.0"), "");
    ASSERT_EQ(specialFilter("0.0.0.0\n2.3.4.5\n6.7.8.9\n0.1.1.1"), "");
    ASSERT_EQ(specialFilter("46.1.1.1\n1.46.3.4\n1.3.46.5\n6.7.8.46"), "46.1.1.1\n1.46.3.4\n1.3.46.5\n6.7.8.46\n");
}