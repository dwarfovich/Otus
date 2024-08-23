#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <map>
#include <forward_list>
#include <array>

int cAllocatorIndex =0;

template<class T>
class CAllocator
{
public:
    using value_type = T;

    int allocatorIndex = 0;

    template<typename U>
    struct rebind
    {
        using other = CAllocator<U>;
    };

    CAllocator()
    { allocatorIndex = ++cAllocatorIndex;
        std::cout << "Created CAllocator #" << allocatorIndex << '\n';
    }
    CAllocator(const CAllocator& other)
    {
        allocatorIndex = ++cAllocatorIndex;
        std::cout << "Created CAllocator #" << allocatorIndex << '\n';
    }

    CAllocator& operator=(const CAllocator& rhs) {}

    CAllocator(CAllocator&&)
    {
        allocatorIndex = ++cAllocatorIndex;
        std::cout << "Created CAllocator #" << allocatorIndex << '\n';
    }
    CAllocator& operator=(CAllocator&&) = default;

    template<class U>
    constexpr CAllocator(const CAllocator<U>& u) noexcept
    {
        allocatorIndex = ++cAllocatorIndex;
        std::cout << "Created CAllocator from U #" << allocatorIndex << '\n';
    }

    [[nodiscard]] T* allocate(std::size_t n) { return (T*)calloc(n, sizeof(T)); }

    void deallocate(T* p, std::size_t n) noexcept { free(p); }
};

class MemoryManager
{
public:
    virtual ~MemoryManager()             = default;
    virtual char* allocate(size_t bytes) = 0;
};

template<std::size_t ChunkSize = 1024>
class ChunkMemoryManager : public MemoryManager
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

int memoryManagerAllocatorIndex = 0;

template<class T>
class MemoryManagerAllocator
{
public:
    using value_type = T;
    static constexpr std::size_t DefaultAllocationSize = 1024;
    int allocatorIndex = 0;

    template<typename U>
    struct rebind
    {
        using other = MemoryManagerAllocator<U>;
    };

    MemoryManagerAllocator() : memory_manager { std::make_shared<ChunkMemoryManager<DefaultAllocationSize>>() }
    {
        allocatorIndex = ++memoryManagerAllocatorIndex;
        std::cout << "Created MemoryManagerAllocator (ctor) #" << allocatorIndex << '\n';
    }
    MemoryManagerAllocator(const std::shared_ptr<MemoryManager>& memoryManager) : memory_manager { memoryManager }
    {
        allocatorIndex = ++memoryManagerAllocatorIndex;
        std::cout << "Created MemoryManagerAllocator (ctor(mm))#" << allocatorIndex << '\n';
        if (!memory_manager) {
            throw std::invalid_argument("memoryManager must have a value");
        }
    }

    MemoryManagerAllocator(const MemoryManagerAllocator& rhs)
    {
        allocatorIndex = ++memoryManagerAllocatorIndex;
        std::cout << "Created MemoryManagerAllocator (copy ctor) #" << allocatorIndex << '\n';
        if (!rhs.memory_manager) {
            memory_manager = std::make_shared<ChunkMemoryManager<DefaultAllocationSize>>();
        }
    }
    MemoryManagerAllocator& operator=(const MemoryManagerAllocator& rhs)
    {
        memory_manager = rhs.memory_manager;
        std::cout << "MemoryManagerAllocator copy assignment\n";
    }

    MemoryManagerAllocator(MemoryManagerAllocator&&)
    {
        allocatorIndex = ++memoryManagerAllocatorIndex;
        std::cout << "Created MemoryManagerAllocator (move ctor) #" << allocatorIndex << '\n';
    }
    MemoryManagerAllocator& operator=(MemoryManagerAllocator&&) = default;

    template<class U>
    constexpr MemoryManagerAllocator(const MemoryManagerAllocator<U>& u) noexcept
    {
        allocatorIndex = ++memoryManagerAllocatorIndex;
        std::cout << "Created MemoryManagerAllocator from U #" << allocatorIndex << '\n';
        memory_manager = u.memoryManager();
    }

    const std::shared_ptr<MemoryManager>& memoryManager() const noexcept { return memory_manager; };

    [[nodiscard]] T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_array_new_length();
        }

        const size_t requiredeSize = n * sizeof(T);
        if (!memory_manager) {
            memory_manager = std::make_shared<ChunkMemoryManager<DefaultAllocationSize>>();
        }
        return (T*)memory_manager->allocate(requiredeSize);
    }

    void deallocate(T* p, std::size_t n) noexcept {}

private:
    std::shared_ptr<MemoryManager> memory_manager = nullptr;
};

template<class T, class U>
bool operator==(const MemoryManagerAllocator<T>&, const MemoryManagerAllocator<U>&)
{
    return true;
}

template<class T, class U>
bool operator!=(const MemoryManagerAllocator<T>&, const MemoryManagerAllocator<U>&)
{
    return false;
}

int main()
{
    
    std::vector<int, CAllocator<int>> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    for (auto i : v) {
        std::cout << i << '\n';
    }

    std::vector<int, MemoryManagerAllocator<int> > vv;
    vv.push_back(10);
    vv.push_back(11);
    for (auto i : vv) {
        std::cout << i << '\n';
    }

    // std::map<int, int, std::greater<int>, ManagedAllocator<std::pair<const int, int>>> map {};
    // map[0] = 0;
    // map[1] = 1;
    // map[2] = 2;
    // for (const auto& [key, value] : map) {
    //     std::cout << key << " - " << value << '\n';
    // }
    // auto v2 = v;
    // v2.push_back(777);
    // for (auto i : v2) {
    //     std::cout << i << '\n';
    // }

    // for (auto i : v) {
    //     std::cout << i << '\n';
    // }

    return 0;
}