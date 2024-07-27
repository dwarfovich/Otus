#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "print_ip.h"

template<typename T>
void print_ip_with_endl(const T& v)
{
    print_ip(v);
    std::cout << std::endl;
}

int main() {
    print_ip_with_endl(2130706433);
    print_ip_with_endl(char(1));
    using namespace std::string_literals;
    print_ip_with_endl("Hello, world!"s);
    std::vector<int> v {44,55,66};
    print_ip_with_endl(v);
    std::list<int> l { 77, 88, 99, 909 };
    print_ip_with_endl(l);
    std::tuple t {9,8,7,6,5};
    print_ip_with_endl(t);
    //std::tuple t2 { 9, true };
    //print_ip(t2);

    return 0;
}
