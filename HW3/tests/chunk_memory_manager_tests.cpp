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
    const std::size_t maxAllocations = 4;
    ChunkMemoryManager<maxAllocations> mm;
    const std::size_t bytes = 1;
    for(size_t i = 0; i < maxAllocations; ++i){
        mm.allocate(bytes);
    }

    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
    const auto& chunk = mm.chunks.front();
    for (size_t i = 1; i < maxAllocations; ++i) {
        EXPECT_EQ(&chunk.memory[i], &chunk.memory[i - 1] + 1);
    }

}