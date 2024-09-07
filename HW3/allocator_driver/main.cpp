#include "allocator/memory_manager_allocator.h"

#include <vector>
#include <map>

int main()
{
    using MapValueType = std::map<std::size_t, std::size_t>::value_type;
    MemoryManagerAllocator<MapValueType, MemoryBank::General> mma;
    std::map<std::size_t, std::size_t, std::greater<>, decltype(mma)> map (mma);

    map[0] = 4;
    std::cout << map[0] << '\n';
    map.get_allocator().getMemoryManager().dump();

    return 0;
}
