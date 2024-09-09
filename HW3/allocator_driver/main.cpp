#include "allocator/memory_manager_allocator.h"

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

int main()
{
    //MemoryManagerAllocator<int> mma;
    //std::vector<int, MemoryManagerAllocator<int>> v;
    //v.push_back(1);
    //mma.getMemoryManager().dump();

    using MapValueType = std::unordered_map<std::size_t, std::size_t>::value_type;
    MemoryManagerAllocator<MapValueType> mma;
    std::unordered_map<std::size_t, std::size_t, std::hash<std::size_t>, std::equal_to<std::size_t>, decltype(mma)> map;
    for (std::size_t i = 0; i < 10; ++i){
        map[i] = i;
        std::cout << map[i] << '\n';
    }

    for (std::size_t i = 0; i < 10; ++i) {
        std::cout << map[i] << '\n';
    }
    //std::cout << map[0] << '\n';
    //map.get_allocator().getMemoryManager().dump();

    return 0;
}
