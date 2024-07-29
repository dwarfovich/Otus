#include <iostream>
#include <map>
#include <array>
#include <utility>
#include <vector>

int allocsCounter = 0;

template<class T>
struct ContiguousAllocator
{
public:
    using value_type = T;

    ContiguousAllocator() noexcept {}
    template<class U>
    ContiguousAllocator(const ContiguousAllocator<U>&) noexcept
    {
    }

    T*   allocate(std::size_t n);
    
    void deallocate(T* p, std::size_t n);

private:
    std::array<T, 10> memory;
    size_t currentPos = 0;
};

int main(int argc, char* argv[])
{
    std::map<int,int,std::greater<int>, ContiguousAllocator<std::pair<const int, int>>> map;
    for (int i : {0, 9}){
        map[i] = i;
    }

    std::cout << "Map contents: \n";
    for (const auto& [key, value] : map){
        std::cout << key << " - " << value << '\n';
    }

    std::cout << "Allocs count: " << allocsCounter << '\n';

    return 0;
}

template<class T, class U>
constexpr bool operator==(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&) noexcept
{
    return true;
}

template<class T, class U>
constexpr bool operator!=(const ContiguousAllocator<T>&, const ContiguousAllocator<U>&) noexcept
{
    return false;
}

template<typename T>
T* ContiguousAllocator<T>::allocate(size_t size)
{
    if (size == 0) {
        return nullptr;
    }

    ++allocsCounter;
    return &memory[currentPos++];
}

template<typename T>
void ContiguousAllocator<T>::deallocate(T* ptr, size_t size)
{
    (void)size;

    if (!ptr) {
        return;
    }
return;
    std::free(ptr);
}
