#include <sstream>
#include <limits>
#include <vector>

#include <gtest/gtest.h>

#include "print_ip.h"

TEST(PrintIpTest, SanityCheck)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    print_ip(int8_t { -1 });
    ASSERT_EQ(outStream.str(), "255");
    outStream.str("");
    print_ip(int16_t { 0 });
    ASSERT_EQ(outStream.str(), "0.0");
    outStream.str("");
    print_ip(int32_t { 2130706433 });
    ASSERT_EQ(outStream.str(), "127.0.0.1");
    outStream.str("");
    print_ip(int64_t { 8875824491850138409 });
    ASSERT_EQ(outStream.str(), "123.45.67.89.101.112.131.41");
    outStream.str("");
    print_ip(std::string { "Hello, World !" });
    ASSERT_EQ(outStream.str(), "Hello, World !");
    outStream.str("");
    print_ip(std::vector<int> { 100, 200, 300, 400 });
    ASSERT_EQ(outStream.str(), "100.200.300.400");
    outStream.str("");
    print_ip(std::list<short> { 400, 300, 200, 100 });
    ASSERT_EQ(outStream.str(), "400.300.200.100");
    outStream.str("");
    print_ip(std::make_tuple(123, 456, 789, 0));
    ASSERT_EQ(outStream.str(), "123.456.789.0");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckUint8)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    print_ip(uint8_t { 0 });
    ASSERT_EQ(outStream.str(), "0");
    outStream.str("");
    print_ip(uint8_t(-1));
    ASSERT_EQ(outStream.str(), "255");
    outStream.str("");
    print_ip(uint8_t { 255 });
    ASSERT_EQ(outStream.str(), "255");
    outStream.str("");
    print_ip(uint8_t(256));
    ASSERT_EQ(outStream.str(), "0");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckInt8)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    print_ip(int8_t { 0 });
    ASSERT_EQ(outStream.str(), "0");
    outStream.str("");
    print_ip(int8_t(-1));
    ASSERT_EQ(outStream.str(), "255");
    outStream.str("");
    print_ip(int8_t { 127 });
    ASSERT_EQ(outStream.str(), "127");
    outStream.str("");
    print_ip(uint8_t(255));
    ASSERT_EQ(outStream.str(), "255");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckUint16)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    print_ip(uint16_t { 0 });
    ASSERT_EQ(outStream.str(), "0.0");
    outStream.str("");
    print_ip(uint16_t(255));
    ASSERT_EQ(outStream.str(), "0.255");
    outStream.str("");
    print_ip(uint16_t(256));
    ASSERT_EQ(outStream.str(), "1.0");
    outStream.str("");
    print_ip(uint16_t(257));
    ASSERT_EQ(outStream.str(), "1.1");
    outStream.str("");
    print_ip(uint16_t(257 + 256));
    ASSERT_EQ(outStream.str(), "2.1");
    outStream.str("");
    print_ip(uint16_t(256 * 255));
    ASSERT_EQ(outStream.str(), "255.0");
    outStream.str("");
    print_ip(uint16_t(256 * 255 + 1));
    ASSERT_EQ(outStream.str(), "255.1");
    outStream.str("");
    print_ip(uint16_t(256 * 255 + 255));
    ASSERT_EQ(outStream.str(), "255.255");
    outStream.str("");
    print_ip(static_cast<uint16_t>(256 * 255 + 255 + 1));
    ASSERT_EQ(outStream.str(), "0.0");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckInt16)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    print_ip(int16_t { 0 });
    ASSERT_EQ(outStream.str(), "0.0");
    outStream.str("");
    print_ip(int16_t(255));
    ASSERT_EQ(outStream.str(), "0.255");
    outStream.str("");
    print_ip(int16_t(256));
    ASSERT_EQ(outStream.str(), "1.0");
    outStream.str("");
    print_ip(int16_t(257));
    ASSERT_EQ(outStream.str(), "1.1");
    outStream.str("");
    print_ip(int16_t(257 + 256));
    ASSERT_EQ(outStream.str(), "2.1");
    outStream.str("");
    print_ip(static_cast<int16_t>(256 * 255));
    ASSERT_EQ(outStream.str(), "255.0");
    outStream.str("");
    print_ip(static_cast<int16_t>(256 * 255 + 1));
    ASSERT_EQ(outStream.str(), "255.1");
    outStream.str("");
    print_ip(static_cast<int16_t>(256 * 255 + 255));
    ASSERT_EQ(outStream.str(), "255.255");
    outStream.str("");
    print_ip(static_cast<int16_t>(256 * 255 + 255 + 1));
    ASSERT_EQ(outStream.str(), "0.0");
    outStream.str("");
    print_ip(static_cast<int16_t>(256 * 255 + 255 + 1 + 1));
    ASSERT_EQ(outStream.str(), "0.1");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckUint64)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    print_ip(uint64_t { 0 });
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.0.0");
    outStream.str("");
    print_ip(uint64_t(255));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.0.255");
    outStream.str("");
    print_ip(uint64_t(256));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.1.0");
    outStream.str("");
    print_ip(uint64_t(257));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.1.1");
    outStream.str("");
    print_ip(uint64_t(257 + 256));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.2.1");
    outStream.str("");
    print_ip(uint64_t(256 * 255 + 255));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.255.255");
    outStream.str("");
    print_ip(uint64_t(-1));
    ASSERT_EQ(outStream.str(), "255.255.255.255.255.255.255.255");
    outStream.str("");
    print_ip(uint64_t(-72'057'594'037'927'936));
    ASSERT_EQ(outStream.str(), "255.0.0.0.0.0.0.0");
    outStream.str("");
    print_ip(uint64_t(-72'057'594'037'927'935));
    ASSERT_EQ(outStream.str(), "255.0.0.0.0.0.0.1");
    outStream.str("");
    print_ip(uint64_t(-281'474'976'710'656));
    ASSERT_EQ(outStream.str(), "255.255.0.0.0.0.0.0");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckInt64)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    print_ip(int64_t { 0 });
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.0.0");
    outStream.str("");
    print_ip(int64_t(255));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.0.255");
    outStream.str("");
    print_ip(int64_t(256));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.1.0");
    outStream.str("");
    print_ip(int64_t(257));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.1.1");
    outStream.str("");
    print_ip(int64_t(257 + 256));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.2.1");
    outStream.str("");
    print_ip(int64_t(256 * 255 + 255));
    ASSERT_EQ(outStream.str(), "0.0.0.0.0.0.255.255");
    outStream.str("");
    print_ip(int64_t(-1));
    ASSERT_EQ(outStream.str(), "255.255.255.255.255.255.255.255");
    outStream.str("");
    print_ip(int64_t(-72'057'594'037'927'936));
    ASSERT_EQ(outStream.str(), "255.0.0.0.0.0.0.0");
    outStream.str("");
    print_ip(int64_t(-72'057'594'037'927'935));
    ASSERT_EQ(outStream.str(), "255.0.0.0.0.0.0.1");
    outStream.str("");
    print_ip(int64_t(-281'474'976'710'656));
    ASSERT_EQ(outStream.str(), "255.255.0.0.0.0.0.0");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckString)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    using namespace std::string_literals;

    print_ip(""s);
    ASSERT_EQ(outStream.str(), "");
    outStream.str("");

    print_ip("1"s);
    ASSERT_EQ(outStream.str(), "1");
    outStream.str("");

    print_ip("abracadabra"s);
    ASSERT_EQ(outStream.str(), "abracadabra");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckVectorOfInts)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    std::vector<int> v = {};
    print_ip(v);
    ASSERT_EQ(outStream.str(), "");
    outStream.str("");

     v = {1};
    print_ip(v);
    ASSERT_EQ(outStream.str(), "1");
    outStream.str("");

    v = { 123, 456, 789, 1011, 1213, 1415, 1617, 1819, 2021, 2223 };
    print_ip(v);
    ASSERT_EQ(outStream.str(), "123.456.789.1011.1213.1415.1617.1819.2021.2223");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckVectorOfStrings)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    std::vector<std::string> v = {};
    print_ip(v);
    ASSERT_EQ(outStream.str(), "");
    outStream.str("");

    v = { "a" };
    print_ip(v);
    ASSERT_EQ(outStream.str(), "a");
    outStream.str("");

    v = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
    print_ip(v);
    ASSERT_EQ(outStream.str(), "a.b.c.d.e.f.g.h.i.j");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckListOfInts)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    std::list<int> list = {};
    print_ip(list);
    ASSERT_EQ(outStream.str(), "");
    outStream.str("");

    list = { 1 };
    print_ip(list);
    ASSERT_EQ(outStream.str(), "1");
    outStream.str("");

    list = { 123, 456, 789, 1011, 1213, 1415, 1617, 1819, 2021, 2223 };
    print_ip(list);
    ASSERT_EQ(outStream.str(), "123.456.789.1011.1213.1415.1617.1819.2021.2223");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckListOfStrings)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    std::list<std::string> list = {};
    print_ip(list);
    ASSERT_EQ(outStream.str(), "");
    outStream.str("");

    list = { "a" };
    print_ip(list);
    ASSERT_EQ(outStream.str(), "a");
    outStream.str("");

    list = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
    print_ip(list);
    ASSERT_EQ(outStream.str(), "a.b.c.d.e.f.g.h.i.j");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckTupleofInts)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    auto tuple = std::make_tuple(1);
    print_ip(tuple);
    ASSERT_EQ(outStream.str(), "1");
    outStream.str("");

    auto tuple2 = std::make_tuple(1,2,3,4,5,6,7,8,9);
    print_ip(tuple2);
    ASSERT_EQ(outStream.str(), "1.2.3.4.5.6.7.8.9");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}

TEST(PrintIpTest, CheckTupleofStrings)
{
    std::stringstream outStream;
    std::streambuf*   oldbuf = std::cout.rdbuf(outStream.rdbuf());

    auto tuple = std::make_tuple("a");
    print_ip(tuple);
    ASSERT_EQ(outStream.str(), "a");
    outStream.str("");

    auto tuple2 = std::make_tuple("a", "b", "c", "d", "e", "f", "g", "h", "i", "j");
    print_ip(tuple2);
    ASSERT_EQ(outStream.str(), "a.b.c.d.e.f.g.h.i.j");
    outStream.str("");

    std::cout.rdbuf(oldbuf);
}