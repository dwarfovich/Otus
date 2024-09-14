#include "allocator/memory_manager_allocator.h"
#include "cc/forward_list.h"

#include <iostream>
#include <vector>
#include <map>

#include <forward_list>

template<typename T>
T factorial(T n)
{
    if (n < 0) {
        throw std::invalid_argument("n have to be greater or equal to 0");
    }

    if (n == 0 || n == 1) {
        return 1;
    }

    T f = 1;
    for (T i = 2; i <= n; ++i) {
        f *= i;
    }

    return f;
}
int main()
{
    std::map<int, int> map;
    for (int i = 0; i < 10; ++i) {
        map[i] = factorial(i);
    }
    std::cout << "Map with std::allocator:\n";
    for (const auto& [key, value] : map) {
        std::cout << key << ": " << value << '\n';
    }

    using ValueType = std::map<int, int>::value_type;
    using MMA       = MemoryManagerAllocator<ValueType>;
    std::map<int, int, std::less<>, MMA> mmamap;
    for (int i = 0; i < 10; ++i) {
        mmamap[i] = factorial(i);
    }
    std::cout << "Map with custom allocator:\n";
    for (const auto& [key, value] : mmamap) {
        std::cout << key << ": " << value << '\n';
    }

    cc::ForwardList<int> list;
    auto iter = list.cbefore_begin();
    for(int i = 0; i < 10; ++i){
        iter = list.insert_after(iter, factorial(i));
    }
    std::cout << "Custom container with std::allocator:\n";
    int counter = 0;
    for( auto i : list){
        std::cout << counter++ << ": " << i << '\n';
    }

    cc::ForwardList<int, MMA> mmList;
    auto                      mmIter = mmList.cbefore_begin();
    for (int i = 0; i < 10; ++i) {
        mmIter = mmList.insert_after(mmIter, factorial(i));
    }
    std::cout << "Custom container with custom allocator:\n";
    int mmCounter = 0;
    for (auto i : mmList) {
        std::cout << mmCounter++ << ": " << i << '\n';
    }

    return 0;
}
