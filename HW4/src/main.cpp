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
    std::tuple<int, int> t;
    print_ip(t);
    std::pair<int, int> p;
    print_ip(p);

    return 0;
}
