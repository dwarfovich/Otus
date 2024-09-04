#include "allocator/memory_manager_allocator.h"

#include <vector>
#include <map>

int main(int argc, char* argv[])
{
    using MMAllocator = MemoryManagerAllocator<int>;
    MMAllocator a;
    auto p1 = a.allocate(1);
    auto        p2 = a.allocate(1);
    a.deallocate(p2, 1);

    /*std::vector<int, MMAllocator> v;
    v.push_back(4);*/
    /*using MMAllocator = MemoryManagerAllocator<int>;
    std::vector<int, MMAllocator> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    for (const auto& i : v) {
        std::cout << i << ' ' << &i << '\n';
    }

    v.erase(v.cbegin() + 1);
    for (const auto& i : v) {
        std::cout << i << ' ' << &i << '\n';
    }
    std::cout << "Capacity: " << v.capacity() << std::endl;

    v.clear();
    v.shrink_to_fit();

    const auto& mm = v.get_allocator();
    MMAllocator mma;
    mma.getMemoryManager().dump();*/
    // v.resize(0);
    // v.shrink_to_fit();

    /*auto v2 = v;
    v2.push_back(2);
    for (const auto& i : v) {
        std::cout << i << ' ' << &i << '\n';
    }

    for (const auto& i : v2) {
        std::cout << i << ' ' << &i << '\n';
    }

    using MapValueType = std::map<int, int>::value_type;
    std::map<int, int, std::greater<int>, MemoryManagerAllocator<MapValueType>> map;
    map[0] = 0;
    map[1] = 1;

    for (const auto& [key, value] : map) {
        std::cout << key << ' ' << value << '\n';
    }*/

    return 0;
}
