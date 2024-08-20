#include <cstdlib>
#include <iostream>
#include <limits>
#include <new>
#include <vector>
#include <map>
#include <memory>
#include <cstring>
#include <forward_list>
#include <array>

int allocatorsCount = 0;
int deallocCount    = 0;
int dctorCount      = 0;

class MemoryManager
{
public:
    virtual ~MemoryManager() = default;
    virtual char* allocate(size_t bytes) = 0;
};

template<std::size_t ChunkSize = 2048>
class ChunkMemoryManager : public MemoryManager
{
public:
    ChunkMemoryManager(){
        /*        std::array<char, ChunkSize> memory;
        std::forward_list<std::pair<size_t, size_t>> freeSlots;
    };
    std::vector<Chunck> chunks;*/
        chunks.push_back({});
        chunks[0].freeSlots.push_front({ 0, ChunkSize });
    }
    ~ChunkMemoryManager() = default;

    char* allocate(size_t bytes)
    {
        for (auto& chunk : chunks) {
            auto& slots = chunk.freeSlots;
            auto  previousIter = slots.before_begin();
            const auto  lastEnd      = slots.cend();
            for (auto iter = slots.begin(); iter != slots.end(); previousIter = iter, ++iter) {
                if (iter->second > bytes) {
                    std::size_t newSlotStart = iter->first + bytes;
                    //std::size_t newSlotFreeBytesCount = iter->first + bytes;
                    auto nextSlot = iter;
                    ++nextSlot;
                    if (nextSlot == lastEnd) {
                        slots.emplace_after(iter, newSlotStart, ChunkSize - newSlotStart - 1);
                    } else{
                        slots.emplace_after(iter, newSlotStart, nextSlot->first - newSlotStart - 1);
                    }
                    char* t = &(chunk.memory[iter->first]);
                    if (previousIter != lastEnd) {
                        slots.erase_after(previousIter);
                    }
                    return t;
                }
            }
        }

        throw std::bad_alloc {};
    }

public:
    struct Chunck
    {
        //std::unique_ptr<char[ChunkSize]>             memory;
        std::array<char, ChunkSize> memory;
        std::forward_list<std::pair<size_t, size_t>> freeSlots;
    };
    std::vector<Chunck> chunks;
};

template<class T>
class ContiguousAllocator
{
public:
    using value_type = T;
    /*typedef T               value_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef const T*        const_pointer;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;
    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type  propagate_on_container_move_assignment;
    typedef std::true_type  propagate_on_container_swap;*/

    int allocatorIndex = 0;
    // using value_type   = T;
    const size_t size = 10;

    template<typename U>
    struct rebind
    {
        typedef ContiguousAllocator<U> other;
    };

    ContiguousAllocator(MemoryManager* memoryManager= nullptr)
    {
        allocatorIndex = ++allocatorsCount;
        if(!memoryManager){
            mm = new ChunkMemoryManager<1000>;
        }
        // memory         = std::make_unique<char[]>(size);
    }

    ContiguousAllocator(const ContiguousAllocator& rhs)
    {
        allocatorIndex = ++allocatorsCount;
        //mm             = std::make_unique<ChunkMemoryManager<1000>>();
        mm = rhs.mm;
        // memory         = std::make_unique<char[]>(size);
        // std::memcpy(memory.get(), rhs.memory.get(), size);
        // currentPos     = rhs.currentPos;
    }
    ContiguousAllocator& operator=(const ContiguousAllocator& rhs)
    {
        mm = rhs.mm;
        // std::memcpy(memory.get(), rhs.memory.get(), size);
        // currentPos = rhs.currentPos;
    }
    ContiguousAllocator(const ContiguousAllocator&&)            = delete;
    ContiguousAllocator& operator=(const ContiguousAllocator&&) = delete;

    template<class U>
    constexpr ContiguousAllocator(const ContiguousAllocator<U>& u) noexcept
    {
        // number = ++counter;
        int t = 432;
        mm = u.mm;
    }

    ~ContiguousAllocator() {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_array_new_length();
        }

        const size_t requiredeSize = n * sizeof(T);
        for (const auto& chunck : chunks) {
        }
        if (!mm) {
            mm = new ChunkMemoryManager<1000>;
        }
        try{
            auto t = (T*)mm->allocate(requiredeSize);
            return t;
        } catch (const std::exception& e){
            auto t = e.what();
            throw std::bad_alloc {};
        }
        
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        
    }

private:
    void report(T* p, std::size_t n, bool alloc = true) const
    {
        std::cout << allocatorIndex << ": " << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n << " bytes at "
                  << std::hex << std::showbase << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    MemoryManager* mm = nullptr;
    struct Chunck
    {
        std::unique_ptr<char[]>                      memory;
        std::forward_list<std::pair<size_t, size_t>> freeSlots;
    };
    std::vector<Chunck> chunks;
    // std::unique_ptr<char[]> memory;
    // size_t                  currentPos = 0;
};

template<class T, class U>
bool operator==(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&)
{
    return true;
}

template<class T, class U>
bool operator!=(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&)
{
    return false;
}

int main()
{
    ChunkMemoryManager m;
    //std::vector<void*> vp;
    //vp.push_back(m.allocate(1));
    //vp.push_back(m.allocate(1));
    //vp.push_back(m.allocate(2));
    //vp.push_back(m.allocate(2));
    //for (const auto& p : vp){
    //    std::cout << p << '\n';
    //}


    auto mm = new ChunkMemoryManager<1000>();
    std::vector<int, ContiguousAllocator<int>> v(0, ContiguousAllocator<int>(mm));
    v.push_back(5);
    v.reserve(10);
    v.push_back(6);
    for (auto i : v) {
        std::cout << i << '\n';
   }

    auto mm2 = std::make_unique<ChunkMemoryManager<1000>>();
    std::map<int, int, std::greater<int>, ContiguousAllocator<std::pair<const int, int>>> map;
    map[0] = 0;
    map[1] = 1;
    map[2] = 2;
    for(const auto& [key, value]: map){
        std::cout << key << " - " << value << '\n';
    }
    //v.reserve(10);
    /* v.push_back(42);
     v.push_back(43);
     v.push_back(44);
     v.push_back(45);
     v.push_back(46);*/
    /*auto v2 = v;
    for (auto i : v2){
        std::cout << i << '\n';
    }*/
    // std::vector<int, ContiguousAllocator<int>> v2 = v;
    // std::cout << v2.size() << '\n';
    /*std::map<int, int, std::greater<int>, ContiguousAllocator<std::pair<const int, int>>> map;
    map[0] = 0;
    map[1] = 1;

    std::vector<int, ContiguousAllocator<int>> v2 = v;
    v2.push_back(5);*/

    //v.clear();
    //v.shrink_to_fit();
    std::cout << "Deallocation count: " << deallocCount << '\n';
    std::cout << "Destructors count: " << dctorCount << '\n';

    return 0;
}