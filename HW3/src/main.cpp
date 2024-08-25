#include <cinttypes>
#include <iostream>
#include <vector>
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
    char* allocate(size_t bytes)
    {
        for (auto& chunk : chunks) {
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
        }

        throw std::bad_alloc {};
    }

private:
    struct Chunk
    {
        Chunk()
        {
            memory.resize(ChunkSize);
            freeSlots.emplace_front(0, ChunkSize);
        }
        std::vector<char>                            memory;
        std::forward_list<std::pair<size_t, size_t>> freeSlots;
    };
    std::vector<Chunk> chunks = { {} };
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

    MemoryManagerAllocator()                              = default;
    MemoryManagerAllocator(const MemoryManagerAllocator&) = default;
    MemoryManagerAllocator& operator=(const MemoryManagerAllocator& rhs) {}

    template<class U, MemoryBank UMemoryBank, typename UMemoryManager>
    constexpr MemoryManagerAllocator(const MemoryManagerAllocator<U, UMemoryBank, UMemoryManager>& u) noexcept
    {
    }

    [[nodiscard]] T* allocate(std::size_t n) { return (T*)memoryManager.allocate(n * sizeof(T)); }

    void deallocate(T* p, std::size_t n) noexcept
    {
        // memoryManager.deallocate(p)
    }

private:
    inline static MemoryManager memoryManager;
};

int main()
{
    using MMAllocator = MemoryManagerAllocator<int>;
    std::vector<int, MMAllocator> v;
    v.push_back(1);
    auto v2 = v;
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

    for(const auto& [key, value] : map){
        std::cout<< key << ' '<< value << '\n';
    }

    std::cout << "Hello World!\n";
}