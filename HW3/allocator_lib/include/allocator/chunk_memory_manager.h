#pragma once

#include <gtest/gtest_prod.h>

#include <cinttypes>
#include <forward_list>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

template<std::size_t ChunkSize = 1024, std::size_t InitialReservedBlocks = 10>
class ChunkMemoryManager
{
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

public:
    char* allocate(std::size_t bytes)
    {
        if (bytes > ChunkSize) [[unlikely]] {
            throw std::runtime_error { "Requested memory is larger than ChunkSize" };
        }

        auto suitableChunk = getSuitableChunk(bytes);
        return allocateInChunk(suitableChunk, bytes);
    }

    void deallocate(char* address, std::size_t size) noexcept
    {
        auto chunkIter = std::find_if(chunks.begin(), chunks.end(), [address](const auto& chunk) {
            return (&chunk.memory.front() <= address) && (&chunk.memory.back() >= address);
        });

        if (chunkIter == chunks.cend()) {
            assert(false && "Something went wrong, this block should not be executed by all means.");
            return;
        }

        while (size != 0) {
            auto&       chunk       = *chunkIter;
            auto        t           = &chunkIter->memory[0] + ChunkSize;
            auto        tt          = address + size;
            std::size_t sizeInBlock = 0;
            if (size_t(address - &chunkIter->memory[0]) + size <= ChunkSize) {
                sizeInBlock = size;
            } else {
                sizeInBlock = ChunkSize - size_t(address - &chunkIter->memory[0]);
            }
            Block newBlock = { (size_t)(address - &chunkIter->memory[0]), sizeInBlock };
            insertFreeBlock(chunk, newBlock);
            ++chunkIter;
            if (chunkIter == chunks.cend()) {
                break;
            }
            size -= sizeInBlock;
            address = &chunkIter->memory[0];
        }
    }

    void dump() const
    {
        std::cout << "Chunks count: " << chunks.size() << '\n';
        int count = 0;
        for (const auto& chunk : chunks) {
            std::cout << "Chunk " << ++count << ":\n";
            int blockNumber = 0;
            for (const auto& block : chunk.allocatedBlocks) {
                std::cout << "    Block " << ++blockNumber << ": size = " << block.size << '\n';
            }
        }
    }

private: // types
    struct Block
    {
        std::size_t startPosition = 0;
        std::size_t size          = 0;
    };
    struct Chunk
    {
        Chunk() : memory(ChunkSize)
        {
            freeBlocks.reserve(InitialReservedBlocks);
            freeBlocks.emplace_back(0, ChunkSize);
        }

        std::vector<char>  memory {};
        std::vector<Block> freeBlocks;
    };
    using BlockIterator = decltype(Chunk::freeBlocks)::iterator;
    struct SuitableChunkData
    {
        Chunk&        chunk;
        BlockIterator freeBlock;
    };
    using SurroundingBlocks = std::pair<BlockIterator, BlockIterator>;

private: // methods
    SuitableChunkData getSuitableChunk(std::size_t bytesRequired)
    {
        for (auto& chunk : chunks) {
            std::size_t freeBytes = 0;
            auto&       blocks    = chunk.freeBlocks;
            for (auto firstBlockIter = blocks.begin(); firstBlockIter != blocks.cend(); ++firstBlockIter) {
                if (firstBlockIter->size >= bytesRequired) {
                    return { chunk, firstBlockIter };
                }
            }
        }

        chunks.push_back({});
        return { chunks.back(), chunks.back().freeBlocks.begin() };
    }

    char* allocateInChunk(SuitableChunkData& suitableChunk, std::size_t bytes)
    {
        auto* memory = &suitableChunk.chunk.memory[suitableChunk.freeBlock->startPosition];
        if (suitableChunk.freeBlock->size == bytes) {
            suitableChunk.chunk.freeBlocks.erase(suitableChunk.freeBlock);
        } else {
            suitableChunk.freeBlock->startPosition += bytes;
            suitableChunk.freeBlock->size -= bytes;
        }
        // std::cout << "Allocated " << bytes << " bytes at " << (size_t)memory << std::endl;
        return memory;
    }

    void insertFreeBlock(Chunk& chunk, Block block)
    {
        bool isRight = false;
        auto& blocks = chunk.freeBlocks;
        auto  prev   = blocks.begin();
        for (auto left = blocks.begin(); left != blocks.end(); ++left) {
            if (left->startPosition + left->size < block.startPosition) {

                prev = left;
            } else {
                break;
            }
        }

        if(prev == blocks.cend()){
            blocks.insert(prev, std::move(block));
            return;
        }

        /*if ( blocks.size() == 1){
            isRight = true;
        }*/
        if (prev->startPosition >= block.startPosition) {
            isRight = true;
        }
        int t = 354;
        if (prev != blocks.cend() && (prev->startPosition + prev->size == block.startPosition)) {
            prev->size += block.size;
        } else {
            if (isRight) {
                prev = blocks.insert(prev, std::move(block));
            } else {
                prev = blocks.insert(prev + 1, std::move(block));
            }
        }

        if (prev != blocks.cend()) {
            auto right = std::next(prev);
            if (right != blocks.cend() && (right->startPosition == prev->startPosition + prev->size)) {
                prev->size += right->size;
                blocks.erase(right);
            }
        }

    }

private: // data
    std::list<Chunk> chunks;
};