#pragma once

#include <gtest/gtest_prod.h>

#include <cinttypes>
#include <cassert>
#include <stdexcept>
#include <list>
#include <vector>
#include <algorithm>

template<std::size_t ChunkSize = 1024, std::size_t InitialReservedBlocks = 10>
class ChunkMemoryManager
{
public:
    char* allocate(std::size_t bytes);
    void  deallocate(char* address, std::size_t size) noexcept;
    void  removeEmptyChunks();

private: // types
    struct Block
    {
        std::size_t endPosition() const;

        std::size_t startPosition = 0;
        std::size_t size          = 0;
    };
    struct Chunk
    {
        Chunk();

        void insertFreeBlock(Block block);

        std::vector<char>  memory;
        std::vector<Block> freeBlocks;
    };
    using BlockIterator = decltype(Chunk::freeBlocks)::iterator;
    struct SuitableChunkData
    {
        Chunk&        chunk;
        BlockIterator freeBlock;
    };

private: // methods
    SuitableChunkData getSuitableChunk(std::size_t bytesRequired);
    char*             allocateInChunk(SuitableChunkData& suitableChunk, std::size_t bytes);

private: // data
    std::list<Chunk> chunks;

private: // test friends
    FRIEND_TEST(ChunkMemoryManagerTest, ChunkMemoryManagerConstructedEmpty);
    FRIEND_TEST(ChunkMemoryManagerTest, ConsequtiveAllocation1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, ConsequtiveAllocation2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, NewChunkAllocation1By1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, NewChunkAllocation2By1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, NewChunkAllocation3By1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromRightToBeginningBy1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromRightToBeginningBy2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromLeftToEndBy1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromLeftToEndBy2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromLeftInMiddleBy1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromLeftInMiddleBy2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromRightInMiddleBy1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, DeallocateFromRightInMiddleBy2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, SparseDeallocationFromLeft1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, SparseDeallocationFromLeft2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, SparseDeallocationFromRight1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, SparseDeallocationFromRight2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, MergingBlocksAfterSparseDeallocationTest);
    FRIEND_TEST(ChunkMemoryManagerTest, MultiChunkAllocation1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, MultiChunkAllocation2ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, MultiChunkDeallocation1ByteTest);
    FRIEND_TEST(ChunkMemoryManagerTest, MultiChunkCrossDeallocationTest);
    FRIEND_TEST(ChunkMemoryManagerTest, RemoveEmptyChunksTest);
};

template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
char* ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::allocate(std::size_t bytes)
{
    if (bytes > ChunkSize) [[unlikely]] {
        throw std::invalid_argument { "Requested memory is larger than ChunkSize" };
    }

    auto suitableChunk = getSuitableChunk(bytes);
    return allocateInChunk(suitableChunk, bytes);
}

template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
void ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::deallocate(char* address, std::size_t size) noexcept
{
    auto chunkIter = std::find_if(chunks.begin(), chunks.end(), [address](const auto& chunk) {
        return (&chunk.memory.front() <= address) && (&chunk.memory.back() >= address);
    });

    if (chunkIter == chunks.cend()) {
        assert(false && "Something went wrong, this block should not be executed by all means.");
        return;
    }

    while (size != 0) {
        const auto distanceToMemoryStart = static_cast<std::size_t>(address - chunkIter->memory.data());
        const auto sizeInBlock = (distanceToMemoryStart + size <= ChunkSize) ? size : ChunkSize - distanceToMemoryStart;
        chunkIter->insertFreeBlock({ distanceToMemoryStart, sizeInBlock });
        if (++chunkIter == chunks.cend()) {
            break;
        }
        size -= sizeInBlock;
        address = chunkIter->memory.data();
    }
}

template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
void ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::removeEmptyChunks()
{
    std::erase_if(chunks, [](const auto& chunk) {
        return chunk.freeBlocks.size() == 1 && chunk.freeBlocks.back().size == ChunkSize;
    });
}

// methods
template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
auto ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::getSuitableChunk(std::size_t bytesRequired)
    -> SuitableChunkData
{
    for (auto& chunk : chunks) {
        auto& blocks = chunk.freeBlocks;
        for (auto firstBlockIter = blocks.begin(); firstBlockIter != blocks.cend(); ++firstBlockIter) {
            if (firstBlockIter->size >= bytesRequired) {
                return { chunk, firstBlockIter };
            }
        }
    }

    chunks.push_back({});
    return { chunks.back(), chunks.back().freeBlocks.begin() };
}

template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
char* ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::allocateInChunk(SuitableChunkData& suitableChunk,
                                                                            std::size_t        bytes)
{
    auto* memory = &suitableChunk.chunk.memory[suitableChunk.freeBlock->startPosition];
    if (suitableChunk.freeBlock->size == bytes) {
        suitableChunk.chunk.freeBlocks.erase(suitableChunk.freeBlock);
    } else {
        suitableChunk.freeBlock->startPosition += bytes;
        suitableChunk.freeBlock->size -= bytes;
    }

    return memory;
}

template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::Chunk::Chunk() : memory(ChunkSize)
{
    freeBlocks.reserve(InitialReservedBlocks);
    freeBlocks.emplace_back(0, ChunkSize);
}

template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
void ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::Chunk::insertFreeBlock(Block block)
{
    auto& blocks        = freeBlocks;
    auto  previousBlock = blocks.begin();
    for (auto left = blocks.begin(); left != blocks.end(); ++left) {
        if (left->endPosition() < block.startPosition) {
            previousBlock = left;
        } else {
            break;
        }
    }

    if (previousBlock == blocks.cend()) {
        blocks.insert(previousBlock, std::move(block));
        return;
    }

    const bool previousBlockIsEnd = (previousBlock == blocks.cend());
    if (!previousBlockIsEnd && (previousBlock->endPosition() == block.startPosition)) {
        previousBlock->size += block.size;
    } else {
        if (previousBlock->startPosition >= block.startPosition) { // previousBlock actually is after target block.
            previousBlock = blocks.insert(previousBlock, std::move(block));
        } else {
            previousBlock = blocks.insert(previousBlock + 1, std::move(block));
        }
    }

    if (!previousBlockIsEnd) {
        const auto rightBlock = std::next(previousBlock);
        if (rightBlock != blocks.cend()
            && (rightBlock->startPosition == previousBlock->endPosition())) {
            previousBlock->size += rightBlock->size;
            blocks.erase(rightBlock);
        }
    }
}

template<std::size_t ChunkSize, std::size_t InitialReservedBlocks>
inline std::size_t ChunkMemoryManager<ChunkSize, InitialReservedBlocks>::Block::endPosition() const
{
    return startPosition + size;
}
