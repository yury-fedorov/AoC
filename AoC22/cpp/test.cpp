#include "benchmark/benchmark.h"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::benchmark::Initialize(&argc, argv);
  // ::benchmark::RunSpecifiedBenchmarks();
  return RUN_ALL_TESTS();
}