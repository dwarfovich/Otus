﻿#include "allocator/memory_manager_allocator.h"
#include "cc/forward_list.h"

#include <iostream>
#include <vector>
#include <map>

#include <forward_list>

template<typename T>
T factorial(T n){
    if(n < 0){
        return 0;
    }

    if(n == 0 || n == 1){
        return 1;
    }

    T f = 1;
    for(T i = 2; i <= n; ++i) {
        f *= i;
    }

    return f;
}
int main()
{
    std::map<int, int> map;
    for (int i = 0; i < 10; ++i){
        map[i]= factorial(i);
    }
    std::cout << "Map with std::allocator:\n";
    for (const auto& [key, value] : map) {
        std::cout << key << ": " << value << '\n';
    }

    using ValueType = std::map<int, int>::value_type;
    using MMA = MemoryManagerAllocator<ValueType>;
    std::map<int, int, std::less<>, MMA> mmamap;
    for (int i = 0; i < 10; ++i) {
        mmamap[i] = factorial(i);
    }
    std::cout << "Map with custom allocator:\n";
    for (const auto& [key, value] : mmamap) {
        std::cout << key << ": " << value << '\n';
    }

    cc::ForwardList<int> list;
    if (list.cbegin() == list.begin() && list.begin() == list.cbegin()){
        std::cout << "Yes\n";
    }
    //auto iter = list.begin();
    //list.insert_after(iter, 77);

    std::forward_list<short> l;
    l.insert_after(l.before_begin(), 88);
    /*std::cout << std::boolalpha << (l.cbegin() == l.cbefore_begin()) << '\n';
    std::cout << sizeof(list) << ' ' << sizeof(l) << '\n';
    std::forward_list<int> l2;
    std::cout << std::boolalpha << (l.cbegin() == l.cend()) << '\n';*/
    
    return 0;
}
