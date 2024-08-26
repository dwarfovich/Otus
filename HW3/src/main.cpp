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
        /* auto firstChunk = std::find_if(chunks.begin(), chunks.end(), [address](const auto& chunk){
             return &chunk.memory[0] <= address && &chunk.memory.back() <= address;
             });*/
    }

private: // types
    struct Chunk
    {
        Chunk() { freeSlots.emplace_front(0, ChunkSize); }

        std::array<char, ChunkSize>                            memory {};
        std::forward_list<std::pair<std::size_t, std::size_t>> freeSlots;
    };

private: // methods
    char* allocateInChunk(Chunk& chunk, std::size_t bytes)
    {
        auto&      slots        = chunk.freeSlots;
        auto       previousIter = slots.before_begin();
        const auto lastEnd      = slots.cend();
        for (auto iter = slots.begin(); iter != slots.end(); previousIter = iter++) {
            if (iter->second > bytes) {
                std::size_t newSlotStart = iter->first + bytes;
                auto        nextSlot     = iter;
                ++nextSlot;
                if (nextSlot == lastEnd) {
                    slots.emplace_after(iter, newSlotStart, ChunkSize - newSlotStart - 1);
                } else {
                    slots.emplace_after(iter, newSlotStart, nextSlot->first - newSlotStart - 1);
                }
                auto* address = &(chunk.memory[iter->first]);
                if (previousIter != lastEnd) {
                    slots.erase_after(previousIter);
                }
                return address;
            }
        }

        return nullptr;
    }

private: // data
    std::list<Chunk> chunks;
};

template<typename T, MemoryBank bank = MemoryBank::General, typename MemoryManager = ChunkMemoryManager<1024>>
class MemoryManagerAllocator
{
public:
    using value_type                  = T;
    using propagate_on_container_swap = std::true_type;
    
    template<typename U>
    struct rebind
    {
        using other = MemoryManagerAllocator<U, bank, MemoryManager>;
    };

    MemoryManagerAllocator()                                             = default;
    MemoryManagerAllocator(const MemoryManagerAllocator&)                = default;
    MemoryManagerAllocator& operator=(const MemoryManagerAllocator& rhs) = default;

    template<class U, MemoryBank UMemoryBank, typename UMemoryManager>
    constexpr MemoryManagerAllocator(const MemoryManagerAllocator<U, UMemoryBank, UMemoryManager>&) noexcept {};

    [[nodiscard]] T* allocate(std::size_t n) { return reinterpret_cast<T*>(memoryManager.allocate(n * sizeof(T))); }

    void deallocate(T* p, std::size_t n) noexcept
    {
        // memoryManager.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
    }

private:
    inline static MemoryManager memoryManager;
};

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
    v.resize(0);
    v.shrink_to_fit();

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