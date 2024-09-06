#include <benchmark/benchmark.h>

int getA(){
    return 6;
}

static void BM_SomeFunction(benchmark::State& state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        getA();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();