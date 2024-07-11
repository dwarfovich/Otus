#include <sstream>
#include <limits>
#include <gtest/gtest.h>

#include "../ip.h"

TEST(IpTest, ValidReadOperatorTest)
{
    std::stringstream s {};
    ip::Ip            ip;

    auto isCorrectReadIp = [&s, &ip](std::string ipStr, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
        s.clear();
        s.str(std::move(ipStr));
        s >> ip;
        return ip.is(p1, p2, p3, p4);
    };

    EXPECT_TRUE(isCorrectReadIp("1.1.1.1", 1,1,1,1));
    EXPECT_TRUE(isCorrectReadIp("0.0.0.0", 0, 0, 0, 0));
    EXPECT_TRUE(isCorrectReadIp("255.255.255.255", 255, 255, 255, 255));
    EXPECT_TRUE(isCorrectReadIp("255.0.1.255", 255, 0, 1, 255));
    EXPECT_TRUE(isCorrectReadIp("0.255.255.0", 0, 255, 255, 0));
    EXPECT_TRUE(isCorrectReadIp("1.255.255.1 ", 1, 255, 255, 1));
    EXPECT_TRUE(isCorrectReadIp("1.2.2.1 2", 1, 2, 2, 1));
    EXPECT_TRUE(isCorrectReadIp("1.2.2.255a", 1, 2, 2, 255));
}

TEST(IpTest, InvalidReadOperatorTest)
{
    std::stringstream s {};
    ip::Ip            ip;

    auto readFailed = [&s, &ip](std::string ipStr) {
        s.clear();
        s.str(std::move(ipStr));
        s >> ip;
        return s.fail();
    };

    EXPECT_TRUE(readFailed(""));
    EXPECT_TRUE(readFailed("1"));
    EXPECT_TRUE(readFailed("255."));
    EXPECT_TRUE(readFailed("255.1"));
    EXPECT_TRUE(readFailed("255.255.255"));
    EXPECT_TRUE(readFailed("255.255.255."));
    EXPECT_TRUE(readFailed("0..0.1"));
    EXPECT_TRUE(readFailed("1...0"));
    EXPECT_TRUE(readFailed("256.0.0.1"));
    EXPECT_TRUE(readFailed("1.333.333.333"));
    EXPECT_TRUE(readFailed("a.1.1.1"));
    EXPECT_TRUE(readFailed("1.a.1.1"));
    EXPECT_TRUE(readFailed("0.0.aa.12"));
    EXPECT_TRUE(readFailed("0.0.12.b"));
    EXPECT_TRUE(readFailed(".1.1.1"));
    EXPECT_TRUE(readFailed(".0.0."));
    EXPECT_TRUE(readFailed("1.1,1.1"));
    EXPECT_TRUE(readFailed("1-1.1.1"));
    EXPECT_TRUE(readFailed("1.1.1:1"));
    EXPECT_TRUE(readFailed("65535.1.1.1"));
    EXPECT_TRUE(readFailed("0.65536.1.1"));
}