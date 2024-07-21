#include <iostream>
#include <vector>

int allocsCounter = 0;

template<class T>
struct LinearAllocator
{
public:
    using value_type = T;

    LinearAllocator() noexcept{}
    template<class U>
    LinearAllocator(const LinearAllocator<U>&) noexcept
    {}

    T*   allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);
};

int main(int argc, char* argv[])
{
    std::vector<int, LinearAllocator<int>> v (5, 7);
    v.push_back(8);
    v.push_back(8);
    v.push_back(8);
    v.push_back(8);
    v.push_back(8);
    v.push_back(8);
    v.push_back(8);

    for(auto i : v){
        std::cout << i << std::endl;
    }
    std::cout << "Allocs count: " << allocsCounter << '\n';

    return 0;
}

template<class T, class U>
constexpr bool operator==(const LinearAllocator<T>&, const LinearAllocator<U>&) noexcept
{
    return true;
}

template<class T, class U>
constexpr bool operator!=(const LinearAllocator<T>&, const LinearAllocator<U>&) noexcept
{
    return false;
}

template<typename T>
T* LinearAllocator<T>::allocate(size_t size)
{
    if (size == 0) {
        return nullptr;
    }

    ++allocsCounter;
    return static_cast<T*>(std::malloc(size * sizeof(T)));
}

template<typename T>
void LinearAllocator<T>::deallocate(T* ptr, size_t size)
{
    (void)size;

    if (!ptr) {
        return;
    }

    std::free(ptr);
}
