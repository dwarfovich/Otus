#include <cstdlib>
#include <iostream>
#include <limits>
#include <new>
#include <vector>
#include <map>
#include <memory>
#include <cstring>

int allocatorsCount = 0;
int deallocCount    = 0;
int dctorCount    = 0;

template<class T>
struct ContiguousAllocator
{
    int allocatorIndex = 0;
    using value_type   = T;
    const size_t size  = 1000;

    ContiguousAllocator()
    {
        allocatorIndex = ++allocatorsCount;
        memory         = std::make_unique<char[]>(size);
    }

    ContiguousAllocator(const ContiguousAllocator& rhs)
    {
        allocatorIndex = ++allocatorsCount;
        memory         = std::make_unique<char[]>(*(rhs.memory.get()));
        currentPos     = rhs.currentPos;
    }
    ContiguousAllocator& operator=(const ContiguousAllocator& rhs)
    {
        std::memcpy(memory.get(), rhs.get(), memory, size);
        currentPos = rhs.currentPos;
    }
    ContiguousAllocator(const ContiguousAllocator&&)            = delete;
    ContiguousAllocator& operator=(const ContiguousAllocator&&) = delete;

    template<class U>
    constexpr ContiguousAllocator(const ContiguousAllocator<U>&) noexcept
    {
        // number = ++counter;
    }

    ~ContiguousAllocator() {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();

        const size_t tSize   = n * sizeof(T);
        const auto   lastPos = currentPos;
        report((T*)&memory[lastPos], n);
        currentPos += tSize;
        return reinterpret_cast<T*>(&memory[lastPos]);
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        report(p, n, 0);
        for (int i = 0; i < n; ++i) {
            p->~T();
            ++p;
            ++dctorCount;
        }
            ++deallocCount;
        // std::free(p);
    }

private:
    void report(T* p, std::size_t n, bool alloc = true) const
    {
        std::cout << allocatorIndex << ": " << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n << " bytes at "
                  << std::hex << std::showbase << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    std::unique_ptr<char[]> memory;
    size_t                  currentPos = 0;
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
    std::vector<int, ContiguousAllocator<int>> v {};
    v.push_back(42);
    v.push_back(43);
    v.push_back(44);
    // std::vector<int, ContiguousAllocator<int>> v2 = v;
    // std::cout << v2.size() << '\n';
    /*std::map<int, int, std::greater<int>, ContiguousAllocator<std::pair<const int, int>>> map;
    map[0] = 0;
    map[1] = 1;

    std::vector<int, ContiguousAllocator<int>> v2 = v;
    v2.push_back(5);*/

    v.clear();
    v.shrink_to_fit();
    std::cout << "Deallocation count: " << deallocCount << '\n';
    std::cout << "Destructors count: " << dctorCount << '\n';

    return 0;
}