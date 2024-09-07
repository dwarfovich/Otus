#include "allocator/memory_manager_allocator.h"

#include <vector>
#include <map>

int main()
{
    //MemoryManagerAllocator<int> mma;
    //std::vector<int, MemoryManagerAllocator<int>> v;
    //v.push_back(1);
    //mma.getMemoryManager().dump();

    using MapValueType = std::map<std::size_t, std::size_t>::value_type;
    MemoryManagerAllocator<MapValueType> mma;
    std::map<std::size_t, std::size_t, std::greater<>, decltype(mma)> map;

    map[0] = 4;
    std::cout << map[0] << '\n';
    map.get_allocator().getMemoryManager().dump();

    return 0;
}
