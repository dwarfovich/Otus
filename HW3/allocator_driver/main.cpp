#include "allocator/memory_manager_allocator.h"

#include <vector>
#include <map>

int main()
{
    using MMAllocator = ChunkMemoryManager<6>;
    MMAllocator a;

    return 0;
}
