#pragma once

#include "memory_bank.h"
#include "chunk_memory_manager.h"

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

    MemoryManagerAllocator()                                             = default;
    MemoryManagerAllocator(const MemoryManagerAllocator&)                = default;
    MemoryManagerAllocator& operator=(const MemoryManagerAllocator& rhs) = default;
    MemoryManagerAllocator(MemoryManagerAllocator&&)                     = default;
    MemoryManagerAllocator& operator=(MemoryManagerAllocator&& rhs)      = default;

    template<class U, MemoryBank UMemoryBank, typename UMemoryManager>
    constexpr MemoryManagerAllocator(const MemoryManagerAllocator<U, UMemoryBank, UMemoryManager>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n) { return reinterpret_cast<T*>(memoryManager.allocate(n * sizeof(T))); }

    void deallocate(T* p, std::size_t n) noexcept
    {
        memoryManager.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
    }

    const MemoryManager& getMemoryManager() const { return memoryManager; };

private:
    inline static MemoryManager memoryManager;
};

template<class T, MemoryBank TMemoryBank, class TMemoryManager, class U, MemoryBank UMemoryBank, class UMemoryManager>
bool operator==(const MemoryManagerAllocator<T, TMemoryBank, TMemoryManager>&,
                const MemoryManagerAllocator<U, UMemoryBank, UMemoryManager>&) noexcept
{
    return std::is_same(T, U) && TMemoryBank == UMemoryBank && std::is_same(TMemoryManager, UMemoryManager);
}