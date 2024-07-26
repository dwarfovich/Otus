#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "print_ip.h"

int main() {
    print_ip(14);
    print_ip(char(1));
    print_ip(std::string("123"));
    std::vector<int> v {44,55,66};
    print_ip(v);
    std::list<int> l { 77, 88, 99, 909 };
    print_ip(l);
    std::tuple t {9,8,7,6,5};
    print_ip(t);
    std::tuple t2 { 9, true };
    print_ip(t2);

    return 0;
}
