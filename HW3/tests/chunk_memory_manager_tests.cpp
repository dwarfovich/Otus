#include "allocator/chunk_memory_manager.h"

#include <gtest/gtest.h>

TEST(ChunkMemoryManagerTest, ChunkMemoryManagerConstructedEmpty)
{
    ChunkMemoryManager mm;
    EXPECT_TRUE(mm.chunks.empty());
}