#include <iostream>
#include <map>
#include <array>
#include <utility>
#include <vector>
#include <memory>

int allocsCounter = 0;

template<class T>
struct ContiguousAllocator
{
public:
    using value_type = T;

    ContiguousAllocator() noexcept {}
   /* template<class U>
    ContiguousAllocator(const ContiguousAllocator<U>&) noexcept
    {
        int t = 34;
    }*/
    ~ContiguousAllocator() noexcept
    {
        if (mem) {
            for (size_t i = 0; i < currentPos; i += sizeof(T)) {
                std::cout << "dctor at: " << (void*)&mem[i] << '\n';
                reinterpret_cast<T*>(&mem[i])->~T();
            }
        }
    }

    T* allocate(std::size_t n)
    {
        if (n == 0) {
            return nullptr;
        }

        ++allocsCounter;
        if (!mem) {
            mem = std::make_unique<char[]>(1000);
            std::cout << "Alloc at: " << (void*)mem.get() << std::endl;
            std::cout << "sizeof pair: " << sizeof(std::pair<int, int>) << std::endl;
            std::cout << "sizeof T: " << sizeof(T) << std::endl;
        }
        std::cout << "allocate for " << n << std::endl;
        auto prevPos = (T*)&mem[currentPos];
        currentPos += sizeof(T) * n;
        std::cout << (void*)prevPos << std::endl;
        return prevPos;
    }

    void deallocate(T* p, std::size_t n) {}

private:
    std::unique_ptr<char[]> mem = nullptr;
    size_t currentPos = 0;
};

int main(int argc, char* argv[])
{
    std::map<int, int, std::greater<int>, ContiguousAllocator<std::pair<const int, int>>> map;
    map[0] = 0;

    for (int i : { 0,1,2,3, 4 }) {
        map[i] = i;
    }

    std::cout << "Map contents: \n";
    for (const auto& [key, value] : map) {
        std::cout << key << " - " << value << '\n';
    }

    std::cout << "Allocs count: " << allocsCounter << '\n';

    return 0;
}

template<class T, class U>
constexpr bool operator==(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&) noexcept
{
    return false;
}

template<class T, class U>
constexpr bool operator!=(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&) noexcept
{
    return true;
}