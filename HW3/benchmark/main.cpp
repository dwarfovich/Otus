#include "allocator/memory_manager_allocator.h"

#include <benchmark/benchmark.h>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <iostream>

inline constexpr std::size_t vectorPuchBacks  = 10'000'000;
inline constexpr std::size_t vectorIterations = 1'000'000;
inline constexpr std::size_t mapInsertions    = 100'000;
inline constexpr std::size_t mapIterations    = 100'000;
inline constexpr std::size_t listInsertions   = 100'000;
inline constexpr std::size_t listIterations   = 100'000;

void BM_Std_VectorPushBack(benchmark::State& state)
{
    std::vector<std::size_t> v;
    for (auto _ : state) {
        state.PauseTiming();
        v.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            v.push_back(i);
        }
    }
}

void BM_MM_VectorPushBack(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::size_t, ChunkMemoryManager<>>;
    std::vector<std::size_t, MMA> v;
    for (auto _ : state) {
        state.PauseTiming();
        v.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            v.push_back(i);
        }
    }
}

void BM_Std_VectorPushBack_WithReservation(benchmark::State& state)
{
    std::vector<std::size_t> v;
    for (auto _ : state) {
        state.PauseTiming();
        v.clear();
        v.reserve(vectorPuchBacks);
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            v.push_back(i);
        }
    }
}

void BM_MM_VectorPushBack_WithReservation(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::size_t, ChunkMemoryManager<vectorPuchBacks * sizeof(std::size_t)>>;
    std::vector<std::size_t, MMA> v;
    for (auto _ : state) {
        state.PauseTiming();
        v.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            v.push_back(i);
        }
    }
}

void BM_Std_VectorIteration(benchmark::State& state)
{
    std::vector<std::size_t> v;
    v.reserve(vectorIterations);
    for (std::size_t i = 0; i < vectorIterations; ++i) {
        v.push_back(i);
    }

    for (auto _ : state) {
        state.PauseTiming();
        std::size_t sum = 0;
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorIterations; ++i) {
            benchmark::DoNotOptimize(sum += v[i]);
        }
    }
}

void BM_MM_VectorIteration(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::size_t, ChunkMemoryManager<vectorIterations * sizeof(std::size_t)>>;
    std::vector<std::size_t, MMA> v;
    for (std::size_t i = 0; i < vectorIterations; ++i) {
        v.push_back(i);
    }

    for (auto _ : state) {
        state.PauseTiming();
        std::size_t sum = 0;
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorIterations; ++i) {
            benchmark::DoNotOptimize(sum += v[i]);
        }
    }
}

void BM_Std_MapInsertion(benchmark::State& state)
{
    std::map<std::size_t, std::size_t> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertions; ++i) {
            map[i] = i;
        }
    }
}

void BM_MM_MapInsertion(benchmark::State& state)
{
    using Valuetype = std::map<std::size_t, std::size_t>::value_type;
    using MMA       = MemoryManagerAllocator<Valuetype, ChunkMemoryManager<>>;
    std::map<std::size_t, std::size_t, std::less<std::size_t>, MMA> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertions; ++i) {
            map[i] = i;
        }
    }
}

void BM_MM_MapInsertion_WithReservation(benchmark::State& state)
{
    using ValueType = std::map<std::size_t, std::size_t>::value_type;
    using MMA       = MemoryManagerAllocator<ValueType, ChunkMemoryManager<mapInsertions * sizeof(ValueType)>>;
    std::map<std::size_t, std::size_t, std::less<std::size_t>, MMA> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertions; ++i) {
            map[i] = i;
        }
    }
}

void BM_Std_MapIteration(benchmark::State& state)
{
    std::map<std::size_t, std::size_t> map;
    for (std::size_t i = 0; i < mapIterations; ++i) {
        map[i] = 1;
    }

    for (auto _ : state) {
        state.PauseTiming();
        std::size_t sum = 0;
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapIterations; ++i) {
            sum += map[i];
        }
    }
}

void BM_MM_MapIteration(benchmark::State& state)
{
    using Valuetype = std::map<std::size_t, std::size_t>::value_type;
    using MMA       = MemoryManagerAllocator<Valuetype, ChunkMemoryManager<>>;
    std::map<std::size_t, std::size_t, std::less<>, MMA> map;
    for (std::size_t i = 0; i < mapIterations; ++i) {
        map[i] = 1;
    }

    for (auto _ : state) {
        state.PauseTiming();
        std::size_t sum = 0;
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapIterations; ++i) {
            sum += map[i];
        }
    }
}

void BM_Std_UMapInsertion(benchmark::State& state)
{
    std::unordered_map<std::size_t, std::size_t> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertions; ++i) {
            map[i] = i;
        }
    }
}

void BM_MM_UMapInsertion(benchmark::State& state)
{
    using Valuetype = std::unordered_map<std::size_t, std::size_t>::value_type;
    using MMA       = MemoryManagerAllocator<Valuetype, ChunkMemoryManager<>>;
    std::unordered_map<std::size_t, std::size_t, std::hash<std::size_t>, std::equal_to<std::size_t>, MMA> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertions; ++i) {
            map[i] = i;
        }
    }
}

void BM_Std_UMapIteration(benchmark::State& state)
{
    std::unordered_map<std::size_t, std::size_t> map;
    for (std::size_t i = 0; i < mapIterations; ++i) {
        map[i] = i;
    }

    for (auto _ : state) {
        state.PauseTiming();
        std::size_t sum = 0;
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapIterations; ++i) {
            benchmark::DoNotOptimize(sum += map[i]);
        }
    }
}

void BM_MM_UMapIteration(benchmark::State& state)
{
    using Valuetype = std::unordered_map<std::size_t, std::size_t>::value_type;
    using MMA       = MemoryManagerAllocator<Valuetype, ChunkMemoryManager<mapIterations * sizeof(Valuetype)>>;
    std::unordered_map<std::size_t, std::size_t, std::hash<std::size_t>, std::equal_to<std::size_t>, MMA> map;
    for (std::size_t i = 0; i < mapIterations; ++i) {
        map[i] = i;
    }

    for (auto _ : state) {
        state.PauseTiming();
        std::size_t sum = 0;
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapIterations; ++i) {
            benchmark::DoNotOptimize(sum += map[i]);
        }
    }
}

void BM_Std_ListInsertion(benchmark::State& state)
{
    std::list<std::size_t> list;
    for (auto _ : state) {
        state.PauseTiming();
        list.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < listInsertions; ++i) {
            list.emplace_back(i);
        }
    }
}

void BM_MM_ListInsertion(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::size_t, ChunkMemoryManager<listInsertions * sizeof(std::size_t)>>;
    std::list<std::size_t, MMA> list;
    for (auto _ : state) {
        state.PauseTiming();
        list.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < listInsertions; ++i) {
            list.emplace_back(i);
        }
    }
}

void BM_Std_ListIteration(benchmark::State& state)
{
    std::list<std::size_t> list;
    for (std::size_t i = 0; i < listIterations; ++i) {
        list.emplace_back(i);
    }

    std::size_t sum = 0;
    for (auto _ : state) {
        state.PauseTiming();
        sum = 0;
        state.ResumeTiming();
        for (auto t : list) {
            benchmark::DoNotOptimize(sum += t);
        }
    }

    if (sum != 0) {
        sum = 0;
    }
}

void BM_MM_ListIteration(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::size_t, ChunkMemoryManager<>>;
    std::list<std::size_t, MMA> list;
    for (std::size_t i = 0; i < listIterations; ++i) {
        list.emplace_back(i);
    }

    std::size_t sum = 0;
    for (auto _ : state) {
        state.PauseTiming();
        sum = 0;
        state.ResumeTiming();
        for (auto t : list) {
            benchmark::DoNotOptimize(sum += t);
        }
    }

    if (sum != 0) {
        sum = 0;
    }
}

// Register the function as a benchmark
BENCHMARK(BM_Std_VectorPushBack);
BENCHMARK(BM_MM_VectorPushBack);
BENCHMARK(BM_Std_VectorPushBack_WithReservation);
BENCHMARK(BM_MM_VectorPushBack_WithReservation);
BENCHMARK(BM_Std_VectorIteration);
BENCHMARK(BM_MM_VectorIteration);
BENCHMARK(BM_Std_MapInsertion);
BENCHMARK(BM_MM_MapInsertion);
BENCHMARK(BM_MM_MapInsertion_WithReservation);
BENCHMARK(BM_Std_MapIteration);
BENCHMARK(BM_MM_MapIteration);
BENCHMARK(BM_Std_UMapInsertion);
BENCHMARK(BM_MM_UMapInsertion);
BENCHMARK(BM_Std_UMapIteration);
BENCHMARK(BM_MM_UMapIteration);
BENCHMARK(BM_Std_ListInsertion);
BENCHMARK(BM_MM_ListInsertion);
BENCHMARK(BM_Std_ListIteration);
BENCHMARK(BM_MM_ListIteration);

// Run the benchmark
BENCHMARK_MAIN();