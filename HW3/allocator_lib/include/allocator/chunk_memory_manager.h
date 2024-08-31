#pragma once

#include <cinttypes>
#include <forward_list>
#include <list>
#include <array>
#include <algorithm>
#include <iostream>

template<std::size_t ChunkSize = 1024>
class ChunkMemoryManager
{
public:
    ChunkMemoryManager() { std::cout << "ChunkMemoryManager()\n"; }
    char* allocate(std::size_t bytes)
    {
        if (bytes > ChunkSize) [[unlikely]] {
            throw std::runtime_error { "Requested memory is larger than ChunkSize" };
        }

        std::cout << "Requested to allocate " << bytes << " bytes\n";
        for (auto& chunk : chunks) {
            auto* allocatedMemory = allocateInChunk(chunk, bytes);
            if (allocatedMemory) {
                std::cout << "Allocated at " << std::hex << (int*)allocatedMemory << '\n';
                return allocatedMemory;
            }
        }

        chunks.push_back({});
        auto* allocatedMemory = allocateInChunk(chunks.back(), bytes);
        if (allocatedMemory) {
            std::cout << "Allocated at " << std::hex << (int*)allocatedMemory << '\n';
            return allocatedMemory;
        } else {
            throw std::bad_alloc {};
        }
    }

    void deallocate(char* address, std::size_t size)
    {
        std::cout << "Requested to deallocate " << size << " bytes at " << std::hex << (int*)address << '\n';
        auto chunkIter = std::find_if(chunks.begin(), chunks.end(), [address](const auto& chunk) {
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
        // Chunk() { allocatedBlocks.emplace_front(0, ChunkSize); }

        std::array<char, ChunkSize> memory {};
        std::forward_list<Block>    allocatedBlocks;
    };

private: // methods
    char* allocateInChunk(Chunk& chunk, std::size_t bytes)
    {
        auto&       blocks        = chunk.allocatedBlocks;
        auto        firstBlock    = blocks.before_begin();
        std::size_t firstPosition = 0;
        auto        secondBlock   = std::next(firstBlock);
        while (secondBlock != blocks.cend() && secondBlock->startPosition - firstPosition < bytes) {
            ++firstBlock;
            ++secondBlock;
            firstPosition = firstBlock->startPosition + firstBlock->size;
        }
        std::size_t secondPosition = secondBlock == blocks.cend() ? ChunkSize : secondBlock->startPosition - 1;
        std::size_t freeBytes      = secondPosition - firstPosition;
        if (freeBytes >= bytes) {
            chunk.allocatedBlocks.insert_after(firstBlock, { firstPosition, bytes });
            return &chunk.memory[firstPosition];
        } else {
            return nullptr;
        }

        return nullptr;
    }

private: // data
    std::list<Chunk> chunks;
};