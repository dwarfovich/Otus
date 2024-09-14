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
        return 0;
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
    

    return 0;
}
