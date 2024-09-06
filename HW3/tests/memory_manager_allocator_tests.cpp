#include "allocator/memory_manager_allocator.h"

#include <gtest/gtest.h>

#include <vector>
#include <map>
#include <list>

TEST(MemoryManagerAllocatorTest, SanityCheckWithVector)
{
    std::vector<int, MemoryManagerAllocator<int>> v;

    const int elementsCount = 10;
    for (int i = 0; i < elementsCount; ++i) {
        v.push_back(i);
    }
    EXPECT_EQ(v.size(), static_cast<std::size_t>(elementsCount));
}

TEST(MemoryManagerAllocatorTest, SanityCheckWithMap)
{
    using MapElementType = std::map<int, int>::value_type;
    std::map<int, int, std::greater<int>, MemoryManagerAllocator<MapElementType>> map;

    const int elementsCount = 10;
    for (int i = 0; i < elementsCount; ++i) {
        map[i] = i;
    }
    EXPECT_EQ(map.size(), static_cast<std::size_t>(elementsCount));
}

TEST(MemoryManagerAllocatorTest, SanityCheckWithList)
{
    using ElementType = std::list<int>::value_type;
    std::list<int, MemoryManagerAllocator<ElementType>> list;

    const int elementsCount = 10;
    for (int i = 0; i < elementsCount; ++i) {
        list.push_back(i);
    }
    EXPECT_EQ(list.size(), static_cast<std::size_t>(elementsCount));
}