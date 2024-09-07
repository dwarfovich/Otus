#include "allocator/memory_manager_allocator.h"

#include <benchmark/benchmark.h>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <iostream>

inline constexpr std::size_t vectorPuchBacks = 1'000'000;

void BM_StdVectorPushBack(benchmark::State& state)
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

void BM_MMVectorPushBack(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::size_t,  ChunkMemoryManager<30'000'000>>;
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

void BM_StdMapInsertion(benchmark::State& state)
{
    std::map<std::size_t, std::size_t> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            map[i] = i;
        }
    }
}

void BM_MMMapInsertion(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::pair<const std::size_t, std::size_t>, ChunkMemoryManager<10'000'000>>;
    std::map<std::size_t, std::size_t, std::less<std::size_t>, MMA> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            map[i] = i;
        }
    }
}

inline const std::size_t mapInsertionElements = 1'000;

void BM_StdMapIteration(benchmark::State& state)
{
    std::map<std::size_t, std::size_t> map;
    for (std::size_t i = 0; i < mapInsertionElements; ++i){
        map[i] = 1;
    }
    std::size_t sum = 0; 
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertionElements; ++i) {
            sum += map[i];
        }
    }
}

void BM_MMMapIteration(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::pair<const std::size_t, std::size_t>, ChunkMemoryManager<10'000'000>>;
    std::map<std::size_t, std::size_t, std::less<>, MMA> map;
    for (std::size_t i = 0; i < mapInsertionElements; ++i) {
        map[i] = 1;
    }
    std::size_t sum = 0; 
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertionElements; ++i) {
            sum += map[i];
        }
    }
}

void BM_StdUMapInsertion(benchmark::State& state)
{
    std::unordered_map<std::size_t, std::size_t> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertionElements; ++i) {
            map[i] = i;
        }
    }
}

void BM_MMUMapInsertion(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::pair<const std::size_t, std::size_t>, ChunkMemoryManager<10'000'000>>;
    std::unordered_map<std::size_t, std::size_t, std::hash<std::size_t>, std::less<std::size_t>, MMA> map;
    for (auto _ : state) {
        state.PauseTiming();
        map.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < mapInsertionElements; ++i) {
            map[i] = i;
        }
    }
}

void BM_StdListInsertion(benchmark::State& state)
{
    std::list<std::size_t> list;
    for (auto _ : state) {
        state.PauseTiming();
        list.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            list.emplace_back(i);
        }
    }
}

void BM_MMListInsertion(benchmark::State& state)
{
    using MMA = MemoryManagerAllocator<std::size_t,  ChunkMemoryManager<10'000'000>>;
    std::list<std::size_t, MMA> list;
    for (auto _ : state) {
        state.PauseTiming();
        list.clear();
        state.ResumeTiming();
        for (std::size_t i = 0; i < vectorPuchBacks; ++i) {
            list.emplace_back(i);
        }
    }
}

// Register the function as a benchmark
BENCHMARK(BM_StdVectorPushBack);
BENCHMARK(BM_MMVectorPushBack);
BENCHMARK(BM_StdMapInsertion);
BENCHMARK(BM_MMMapInsertion);
BENCHMARK(BM_StdMapIteration);
BENCHMARK(BM_MMMapIteration);
BENCHMARK(BM_StdUMapInsertion);
BENCHMARK(BM_MMUMapInsertion);


BENCHMARK(BM_StdListInsertion);
BENCHMARK(BM_MMListInsertion);
// Run the benchmark
BENCHMARK_MAIN();