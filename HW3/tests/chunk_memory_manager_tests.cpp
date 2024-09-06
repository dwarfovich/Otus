#include "allocator/chunk_memory_manager.h"

#include <gtest/gtest.h>

#include <array>

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
    const std::size_t                          maxAllocations = 4;
    const std::size_t                          bytes          = 2;
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

TEST(ChunkMemoryManagerTest, NewChunkAllocation1By1ByteTest)
{
    const std::size_t         bytes = 1;
    ChunkMemoryManager<bytes> mm;

    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 2);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 3);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
}

TEST(ChunkMemoryManagerTest, NewChunkAllocation2By1ByteTest)
{
    const std::size_t         bytes = 2;
    ChunkMemoryManager<bytes> mm;

    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 2);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 3);
    EXPECT_EQ(mm.chunks.front().freeBlocks.size(), 0);
}

TEST(ChunkMemoryManagerTest, NewChunkAllocation3By1ByteTest)
{
    const std::size_t             bytes = 1;
    ChunkMemoryManager<bytes * 3> mm;

    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 1);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 0);

    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 2);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 2);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 2);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 0);

    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 3);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 3);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
    mm.allocate(bytes);
    EXPECT_EQ(mm.chunks.size(), 3);
    EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 0);
}

TEST(ChunkMemoryManagerTest, AllocationExceptionTest)
{
    const std::size_t             chunkSize = 10;
    ChunkMemoryManager<chunkSize> mm;

    EXPECT_THROW(mm.allocate(chunkSize + 1), std::exception);
}

TEST(ChunkMemoryManagerTest, DeallocateFromRightToBeginningBy1ByteTest)
{
    const std::size_t             chunkSize = 4;
    ChunkMemoryManager<chunkSize> mm;
    
    const std::size_t             allocations = 4;
    std::array<char*, allocations> addresses;
    for (std::size_t i = 0; i < allocations; ++i){
        addresses[i] = mm.allocate(1);
    }

    for (std::size_t i = allocations - 1; i < allocations; --i) {
        mm.deallocate(addresses[i], 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().startPosition, i);
    }
}

TEST(ChunkMemoryManagerTest, DeallocateFromLeftToEndBy1ByteTest)
{
    const std::size_t allocationSize = 1;             
    const std::size_t             chunkSize = 4 * allocationSize;
    ChunkMemoryManager<chunkSize> mm;

    const std::size_t              allocations = 4;
    std::array<char*, allocations> addresses;
    for (std::size_t i = 0; i < allocations; ++i) {
        addresses[i] = mm.allocate(allocationSize);
    }

    for (std::size_t i = 0; i < allocations; ++i) {
        mm.deallocate(addresses[i], allocationSize);
        EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().startPosition, 0);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().size, i*allocationSize + allocationSize);
    }
}

TEST(ChunkMemoryManagerTest, DeallocateFromLeftToEndBy2ByteTest)
{
    const std::size_t             allocationSize = 2;
    const std::size_t             chunkSize      = 4 * allocationSize;
    ChunkMemoryManager<chunkSize> mm;

    const std::size_t              allocations = 4;
    std::array<char*, allocations> addresses;
    for (std::size_t i = 0; i < allocations; ++i) {
        addresses[i] = mm.allocate(allocationSize);
    }

    for (std::size_t i = 0; i < allocations; ++i) {
        mm.deallocate(addresses[i], allocationSize);
        EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().startPosition, 0);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().size, i * allocationSize + allocationSize);
    }
}

TEST(ChunkMemoryManagerTest, DeallocateFromLeftInMiddleBy1ByteTest)
{
    const std::size_t             allocationSize = 1;
    const std::size_t             chunkSize      = 8 * allocationSize;
    ChunkMemoryManager<chunkSize> mm;

    const std::size_t              allocations = 8;
    std::array<char*, allocations> addresses;
    for (std::size_t i = 0; i < allocations; ++i) {
        addresses[i] = mm.allocate(allocationSize);
    }

    for (std::size_t i = 3; i < allocations; ++i) {
        mm.deallocate(addresses[i], allocationSize);
        EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().startPosition, 3);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().size, i * allocationSize + allocationSize - allocationSize*3);
    }
}

TEST(ChunkMemoryManagerTest, DeallocateFromLeftInMiddleBy2ByteTest)
{
    const std::size_t             allocationSize = 2;
    const std::size_t             chunkSize      = 8 * allocationSize;
    ChunkMemoryManager<chunkSize> mm;

    const std::size_t              allocations = 8;
    std::array<char*, allocations> addresses;
    for (std::size_t i = 0; i < allocations; ++i) {
        addresses[i] = mm.allocate(allocationSize);
    }

    for (std::size_t i = 3; i < allocations; ++i) {
        mm.deallocate(addresses[i], allocationSize);
        EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().startPosition, 6);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().size, i * allocationSize + allocationSize - allocationSize * 3);
    }
}

TEST(ChunkMemoryManagerTest, DeallocateFromRightInMiddleBy1ByteTest)
{
    const std::size_t             allocationSize = 1;
    const std::size_t             chunkSize      = 8 * allocationSize;
    ChunkMemoryManager<chunkSize> mm;

    const std::size_t              allocations = 8;
    std::array<char*, allocations> addresses;
    for (std::size_t i = 0; i < allocations; ++i) {
        addresses[i] = mm.allocate(allocationSize);
    }


    for (std::size_t i = 6, freeSize = allocationSize; i < allocations; --i, ++freeSize) {
        mm.deallocate(addresses[i], allocationSize);
        EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().startPosition, i);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().size, freeSize);
    }
}

TEST(ChunkMemoryManagerTest, DeallocateFromRightInMiddleBy2ByteTest)
{
    const std::size_t             allocationSize = 2;
    const std::size_t             chunkSize      = 8 * allocationSize;
    ChunkMemoryManager<chunkSize> mm;

    const std::size_t              allocations = 8;
    std::array<char*, allocations> addresses;
    for (std::size_t i = 0; i < allocations; ++i) {
        addresses[i] = mm.allocate(allocationSize);
    }

    for (std::size_t i = 6, freeSize = allocationSize; i < allocations; --i, freeSize += 2) {
        mm.deallocate(addresses[i], allocationSize);
        EXPECT_EQ(mm.chunks.back().freeBlocks.size(), 1);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().startPosition, i * allocationSize);
        EXPECT_EQ(mm.chunks.back().freeBlocks.back().size, freeSize);
    }
}