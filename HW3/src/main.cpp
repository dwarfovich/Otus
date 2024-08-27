#include <cinttypes>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <forward_list>
#include <array>
#include <map>

enum class MemoryBank : std::uint8_t
{
    General,
    Audio,
    Video
};

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

        for (auto& chunk : chunks) {
            auto* allocatedMemory = allocateInChunk(chunk, bytes);
            if (allocatedMemory) {
                return allocatedMemory;
            }
        }

        chunks.push_back({});
        auto* allocatedMemory = allocateInChunk(chunks.back(), bytes);
        if (allocatedMemory) {
            return allocatedMemory;
        } else {
            throw std::bad_alloc {};
        }
    }

    void deallocate(char* address, size_t size)
    {
        auto chunkIter = std::find_if(chunks.begin(), chunks.end(), [address](const auto& chunk) {
            return &chunk.memory[0] <= address && &chunk.memory.back() <= address;
        });
        if (chunkIter == chunks.cend()) [[unlikely]] {
            throw std::runtime_error("Wrong address provided for deallocation");
        }

    }

private: // types
    struct Block
    {
        std::size_t startPosition = 0;
        std::size_t freeBytes     = 0;
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
            firstPosition = firstBlock->startPosition;
        }
        std::size_t secondPosition =
            secondBlock == blocks.cend() ? ChunkSize : secondBlock->startPosition - 1;
        std::size_t freeBlocks = secondPosition - firstPosition;
        if(freeBlocks >= bytes){
            chunk.allocatedBlocks.insert_after(firstBlock, {firstPosition, bytes});
            return &chunk.memory[firstPosition];
        } else{
            return nullptr;
        }
        /*auto&      blocks       = chunk.freeBlocks;
        auto       previousIter = blocks.before_begin();
        const auto lastEnd      = blocks.cend();
        for (auto iter = blocks.begin(); iter != blocks.end(); previousIter = iter++) {
            if (iter->freeBytes > bytes) {
                std::size_t newBlockStart = iter->startPosition + bytes;
                auto        nextBlock     = iter;
                ++nextBlock;
                if (nextBlock == lastEnd) {
                    blocks.emplace_after(iter, newBlockStart, ChunkSize - newBlockStart - 1);
                } else {
                    blocks.emplace_after(iter, newBlockStart, nextBlock->startPosition - newBlockStart - 1);
                }
                auto* address = &(chunk.memory[iter->startPosition]);
                if (previousIter != lastEnd) {
                    blocks.erase_after(previousIter);
                }
                return address;
            }
        }*/

        return nullptr;
    }

private: // data
    std::list<Chunk> chunks;
};

template<typename T, MemoryBank bank = MemoryBank::General, typename MemoryManager = ChunkMemoryManager<1024>>
class MemoryManagerAllocator
{
public:
    using value_type = T;

    template<typename U>
    struct rebind
    {
        using other = MemoryManagerAllocator<U, bank, MemoryManager>;
    };

    MemoryManagerAllocator()                                             = default;
    MemoryManagerAllocator(const MemoryManagerAllocator&)                = default;
    MemoryManagerAllocator& operator=(const MemoryManagerAllocator& rhs) = default;
    MemoryManagerAllocator(MemoryManagerAllocator&&)                     = default;
    MemoryManagerAllocator& operator=(MemoryManagerAllocator&& rhs)      = default;

    template<class U, MemoryBank UMemoryBank, typename UMemoryManager>
    constexpr MemoryManagerAllocator(const MemoryManagerAllocator<U, UMemoryBank, UMemoryManager>&) noexcept {};

    [[nodiscard]] T* allocate(std::size_t n) { return reinterpret_cast<T*>(memoryManager.allocate(n * sizeof(T))); }

    void deallocate(T* p, std::size_t n) noexcept
    {
        memoryManager.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
    }

private:
    inline static MemoryManager memoryManager;
};

// TODO: reconcider return value.
template<class T, MemoryBank TMemoryBank, class TMemoryManager, class U, MemoryBank UMemoryBank, class UMemoryManager>
bool operator==(const MemoryManagerAllocator<T, TMemoryBank, TMemoryManager>&,
                const MemoryManagerAllocator<U, UMemoryBank, UMemoryManager>&)
{
    return true;
}

template<class T, MemoryBank TMemoryBank, class TMemoryManager, class U, MemoryBank UMemoryBank, class UMemoryManager>
bool operator!=(const MemoryManagerAllocator<T, TMemoryBank, TMemoryManager>&,
                const MemoryManagerAllocator<U, UMemoryBank, UMemoryManager>&)
{
    return false;
}

int main()
{
    using MMAllocator = MemoryManagerAllocator<int>;
    std::vector<int, MMAllocator> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    for (const auto& i : v) {
        std::cout << i << ' ' << &i << '\n';
    }

    v.erase(v.cbegin() + 1);
    for (const auto& i : v) {
        std::cout << i << ' ' << &i << '\n';
    }
    //v.resize(0);
    //v.shrink_to_fit();

    /*auto v2 = v;
    v2.push_back(2);
    for (const auto& i : v) {
        std::cout << i << ' ' << &i << '\n';
    }

    for (const auto& i : v2) {
        std::cout << i << ' ' << &i << '\n';
    }

    using MapValueType = std::map<int, int>::value_type;
    std::map<int, int, std::greater<int>, MemoryManagerAllocator<MapValueType>> map;
    map[0] = 0;
    map[1] = 1;

    for (const auto& [key, value] : map) {
        std::cout << key << ' ' << value << '\n';
    }*/

    std::cout << "Hello World!\n";
}