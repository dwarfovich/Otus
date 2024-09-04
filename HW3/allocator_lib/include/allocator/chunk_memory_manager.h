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
            // std::size_t sizeInBlock = (&chunkIter->memory[0] + ChunkSize <= address + size)
            //                               ? size
            //                               : &chunkIter->memory[0] + ChunkSize - address;
            Block newBlock = { (size_t)(address - &chunkIter->memory[0]), sizeInBlock };
            //std::cout << "Deallocating " << size << " at " << (size_t)address << std::endl;
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
        //std::cout << "Allocated " << bytes << " bytes at " << (size_t)memory << std::endl;
        return memory;
    }

    void insertFreeBlock(Chunk& chunk, Block block) {}

    SurroundingBlocks findSurroundingFreeBlocks(
        char* startAddress, char* targetAddress, std::size_t targetSize, BlockIterator first, BlockIterator end)
    {
        auto previousBlock = end;
        while (first != end) {
            if (startAddress + first->startPosition + first->size == targetAddress) {
                previousBlock = first;
                break;
            } else {
                break;
            }
        }
        auto nextBlock = previousBlock + 1;
        while (nextBlock != end) {
        }
    }

private: // data
    std::list<Chunk> chunks;
};