#include "backend.hpp"
#include "vector_ops.hpp"

#include <benchmark/benchmark.h>

#include <cstddef>
#include <vector>

namespace
{
void benchmark_vector_add(benchmark::State& state, Backend backend)
{
    // The benchmark size comes from BENCHMARK(...)->Arg(...).
    const std::size_t size = static_cast<std::size_t>(state.range(0));

    std::vector<float> a(size, 1.0f);
    std::vector<float> b(size, 2.0f);
    std::vector<float> out(size);

    // Google Benchmark repeats this loop enough times to measure stable timings.
    // Standard style uses "_", a variable name that signals "intentionally unused".
    // More explicit equivalent:
    // for (auto benchmark_iteration : state)
    // {
    //     (void)benchmark_iteration;
    //     vector_add(...);
    // }
    for (auto _ : state)
    {
        vector_add(backend, a.data(), b.data(), out.data(), size);

        // Prevent the compiler from removing the computation as "unused".
        benchmark::DoNotOptimize(out.data());

        // Tell the compiler that memory may have changed after this point.
        benchmark::ClobberMemory();
    }

    // Report how many vector elements were processed in total.
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(size));
}

void vector_add_cpu_benchmark(benchmark::State& state)
{
    benchmark_vector_add(state, Backend::Cpu);
}

#ifdef ENABLE_SIMD
void vector_add_simd_benchmark(benchmark::State& state)
{
    benchmark_vector_add(state, Backend::Simd);
}
#endif
}

// Register the CPU benchmark.
// Each Arg value is passed to state.range(0) and becomes the vector size.
BENCHMARK(vector_add_cpu_benchmark)
    ->Arg(1024)
    ->Arg(1024 * 1024);

#ifdef ENABLE_SIMD
// Register the SIMD benchmark with the same sizes as CPU for comparison.
BENCHMARK(vector_add_simd_benchmark)
    ->Arg(1024)
    ->Arg(1024 * 1024);
#endif
