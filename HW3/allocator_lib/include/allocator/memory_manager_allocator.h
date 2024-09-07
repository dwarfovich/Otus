#pragma once

#include "memory_bank.h"
#include "chunk_memory_manager.h"

template<typename T, typename MemoryManager = ChunkMemoryManager<1024>, MemoryBank bank = MemoryBank::General>
class MemoryManagerAllocator
{
public:
    using value_type = T;

    template<typename U>
    struct rebind
    {
        using other = MemoryManagerAllocator<U, MemoryManager, bank>;
    };

    MemoryManagerAllocator()                                             = default;
    MemoryManagerAllocator(const MemoryManagerAllocator&)                = default;
    MemoryManagerAllocator& operator=(const MemoryManagerAllocator& rhs) = default;
    MemoryManagerAllocator(MemoryManagerAllocator&&)                     = default;
    MemoryManagerAllocator& operator=(MemoryManagerAllocator&& rhs)      = default;

    template<class U, typename UMemoryManager, MemoryBank UMemoryBank>
    constexpr MemoryManagerAllocator(const MemoryManagerAllocator<U, UMemoryManager, UMemoryBank>&) noexcept
    {
    }

    [[nodiscard]] T* allocate(std::size_t n) { return reinterpret_cast<T*>(memoryManager.allocate(n * sizeof(T))); }

    void deallocate(T* p, std::size_t n) noexcept
    {
        memoryManager.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
    }

    const MemoryManager& getMemoryManager() const { return memoryManager; };

private:
    inline static MemoryManager memoryManager;
};

template<class T, class TMemoryManager, MemoryBank TMemoryBank, class U, class UMemoryManager, MemoryBank UMemoryBank>
bool operator==(const MemoryManagerAllocator<T, TMemoryManager, TMemoryBank>&,
                const MemoryManagerAllocator<U, UMemoryManager, UMemoryBank>&) noexcept
{
    return std::is_same<T, U>() && TMemoryBank == UMemoryBank && std::is_same<TMemoryManager, UMemoryManager>();
}