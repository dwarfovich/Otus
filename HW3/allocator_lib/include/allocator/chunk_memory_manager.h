#pragma once

#include <gtest/gtest_prod.h>

#include <cinttypes>
#include <forward_list>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

template<std::size_t ChunkSize = 1024, std::size_t InitialReservedBlocks = 10>
class ChunkMemoryManager
{
    FRIEND_TEST(ChunkMemoryManagerTest, ChunkMemoryManagerConstructedEmpty);
    FRIEND_TEST(ChunkMemoryManagerTest, ConsequtiveAllocation1ByteTest);

public:
    char* allocate(std::size_t bytes)
    {
        if (bytes > ChunkSize) [[unlikely]] {
            throw std::runtime_error { "Requested memory is larger than ChunkSize" };
        }

        auto suitableChunk = getSuitableChunk(bytes);
        return allocateInChunk(suitableChunk, bytes);
    }

    void deallocate(char* address, std::size_t size)
    {
        /*auto chunkIter = std::find_if(chunks.begin(), chunks.end(), [address](const auto& chunk) {
            std::cout << (int*)&chunk.memory[0] << ' ' << (int*)&chunk.memory.back() << '\n';
            return (&chunk.memory[0] <= address) && (&chunk.memory.back() >= address);
        });
        if (chunkIter == chunks.cend()) [[unlikely]] {
            throw std::runtime_error("Wrong address provided for deallocation");
        }
        auto& blocks = chunkIter->allocatedBlocks;
        for (auto iter = blocks.begin(), iterBefore = blocks.before_begin(); iter != blocks.cend() && size != 0;) {
            if (iter->size <= size) {
                size -= iter->size;
                iter = blocks.erase_after(iterBefore);
            } else {
                iter->startPosition += size;
                size = 0;
            }
        }*/
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
        return { chunks.front(), chunks.front().freeBlocks.begin() };
    }

    char* allocateInChunk(SuitableChunkData& suitableChunk, std::size_t bytes)
    {
        auto* memory = &suitableChunk.chunk.memory[suitableChunk.freeBlock->startPosition];
        if (suitableChunk.freeBlock->size == bytes) {
            suitableChunk.chunk.freeBlocks.erase(suitableChunk.freeBlock);
        } else{
            suitableChunk.freeBlock->startPosition += bytes;
            suitableChunk.freeBlock->size -= bytes;
        }
        
        return memory;
    }

private: // data
    std::list<Chunk> chunks;
};