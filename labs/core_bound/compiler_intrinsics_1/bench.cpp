
#include "benchmark/benchmark.h"
#include "solution.h"
#include <memory>

#define SOLUTION 3 // Define SOLUTION as a preprocessor constant

static void bench_partial_sum(benchmark::State &state) {
  InputVector inA;
  init(inA);

  OutputVector outB;
  zero(outB, (int)inA.size());

  for (auto _ : state) {
    #if SOLUTION == 0
      // Baseline.
      imageSmoothing(inA, radius, outB);
    #elif SOLUTION == 1
      // My solution.
      mySolution(inA, radius, outB);
    #elif SOLUTION == 2
      // My solution A.
      mySolutionA(inA, radius, outB);
    #elif SOLUTION == 3
      // Facit.
      facit(inA, radius, outB);
    #else
      #  pragma error("Unknown solution. Valid values are 0 through 2.")
    #endif
    benchmark::DoNotOptimize(outB);
  }
}

static void bench_mySolution(benchmark::State &state) {
  InputVector inA;
  init(inA);

  OutputVector outB;
  zero(outB, (int)inA.size());

  for (auto _ : state) {
      mySolution(inA, radius, outB);
    benchmark::DoNotOptimize(outB);
  }
}

static void bench_mySolutionA(benchmark::State &state) {
  InputVector inA;
  init(inA);

  OutputVector outB;
  zero(outB, (int)inA.size());

  for (auto _ : state) {
      mySolutionA(inA, radius, outB);
    benchmark::DoNotOptimize(outB);
  }
}

static void bench_facit(benchmark::State &state) {
  InputVector inA;
  init(inA);

  OutputVector outB;
  zero(outB, (int)inA.size());

  for (auto _ : state) {
      facit(inA, radius, outB);
    benchmark::DoNotOptimize(outB);
  }
}

// Register the function as a benchmark
// BENCHMARK(bench_partial_sum)->Unit(benchmark::kMicrosecond);
// BENCHMARK(bench_mySolution)->Unit(benchmark::kMicrosecond);
BENCHMARK(bench_mySolutionA)->Unit(benchmark::kMicrosecond);
// BENCHMARK(bench_facit)->Unit(benchmark::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();
