#include "allocator/chunk_memory_manager.h"

#include <gtest/gtest.h>

#include <algorithm>

TEST(ChunkMemoryManagerTest, ChunkMemoryManagerConstructedEmpty)
{
    ChunkMemoryManager mm;
    EXPECT_TRUE(mm.chunks.empty());
}

TEST(ChunkMemoryManagerTest, ConsequtiveAllocation1ByteTest)
{
    const std::size_t                          maxAllocations = 4;
    const std::size_t                          bytes          = 1;
    ChunkMemoryManager<maxAllocations * bytes> mm;

    char* previousAddress = 0;
    for (size_t i = 0; i < maxAllocations; ++i) {
        auto* address = mm.allocate(bytes);
        if (i != 0) {
            EXPECT_EQ(previousAddress + bytes, address);
        }
        previousAddress = address;
    }

    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
}

TEST(ChunkMemoryManagerTest, ConsequtiveAllocation2ByteTest)
{
    const std::size_t                  maxAllocations = 4;
    const std::size_t                  bytes = 2;
    ChunkMemoryManager<maxAllocations * bytes> mm;

    char* previousAddress = 0;
    for (size_t i = 0; i < maxAllocations; ++i) {
        auto* address = mm.allocate(bytes);
        if (i!=0){
            EXPECT_EQ(previousAddress + bytes, address);
        }
        previousAddress = address;
    }

    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
}