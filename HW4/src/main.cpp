#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "print_ip.h"

/*! \mainpage Otus Homework 4 documentation
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 */

template<typename T>
void print_ip_with_endl(const T& v)
{
    print_ip(v);
    std::cout << std::endl;
}

int main()
{
    print_ip_with_endl(int8_t { -1 });                           // 255
    print_ip_with_endl(int16_t { 0 });                           // 0.0
    print_ip_with_endl(int32_t { 2130706433 });                  // 127.0.0.1
    print_ip_with_endl(int64_t { 8875824491850138409 });         // 123.45.67.89.101.112.131.41
    print_ip_with_endl(std::string { "Hello, World !" });        // Hello, World!
    print_ip_with_endl(std::vector<int> { 100, 200, 300, 400 }); // 100.200.300.400
    print_ip_with_endl(std::list<short> { 400, 300, 200, 100 }); // 400.300.200.100
    print_ip_with_endl(std::make_tuple(123, 456, 789, 0));       // 123.456.789.0

    // std::tuple t2 { 9, true };
    // print_ip(t2); // Fail to compile.

    return 0;
}
