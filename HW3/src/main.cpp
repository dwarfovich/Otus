#include <cstdlib>
#include <iostream>
#include <limits>
#include <new>
#include <vector>
#include <map>

inline static int allocatorsCount = 0;

template<class T>
struct Mallocator
{
    int       allocatorIndex = 0;
    typedef T value_type;
    const size_t    size = 1000;

    Mallocator()
    {
        allocatorIndex = ++allocatorsCount;
        memory         = (char*)malloc(size);
    }

    Mallocator(const Mallocator& rhs){
        allocatorIndex = ++allocatorsCount;
        memory         = (char*)malloc(size);
    }
    Mallocator& operator=(const Mallocator&){
        
    }
    Mallocator(const Mallocator&&)     {}
    Mallocator& operator=(const Mallocator&&) {}

    template<class U>
    constexpr Mallocator(const Mallocator<U>&) noexcept
    {
        // number = ++counter;
    }

    ~Mallocator()
    {
        if (!memory) {
            return;
        }
        for (size_t i = 0; i < currentPos; i += sizeof(T)) {
            ((T*)&memory[i])->~T();
        }

        free(memory);
    }

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
        //std::free(p);
    }

private:
    void report(T* p, std::size_t n, bool alloc = true) const
    {
        std::cout << allocatorIndex << ": " << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n << " bytes at "
                  << std::hex << std::showbase << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    char*  memory     = nullptr;
    size_t currentPos = 0;
};

template<class T, class U>
bool operator==(const Mallocator<T>&, const Mallocator<U>&)
{
    return true;
}

template<class T, class U>
bool operator!=(const Mallocator<T>&, const Mallocator<U>&)
{
    return false;
}

int main()
{
    std::vector<int, Mallocator<int>> v(8);
    v.push_back(42);
   /* std::map<int, int, std::greater<int>, Mallocator<std::pair<const int, int>>> map;
    map[0] = 0;
    map[1] = 1;*/

    return 0;
}