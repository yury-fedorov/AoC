#include "absl/container/flat_hash_set.h"
#include "benchmark/benchmark.h"
#include "common.h"
#include <set>
#include <unordered_set>

namespace day06 {
constexpr auto test = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";
constexpr auto sequence_length_1 = 4;
constexpr auto sequence_length_2 = 14;

bool IsUniqueSet(const char *b, size_t sequence_length) {
  const char *e = b + sequence_length;
  std::set<char> s(b, e);
  return s.size() == sequence_length;
}

bool IsUniqueUnorderedSet(const char *b, size_t sequence_length) {
  const char *e = b + sequence_length;
  std::unordered_set<char> s(b, e);
  return s.size() == sequence_length;
}

bool IsUniqueAbslFlatHashSet(const char *b, size_t sequence_length) {
  const char *e = b + sequence_length;
  absl::flat_hash_set<char> s(b, e);
  return s.size() == sequence_length;
}

bool IsUniqueVectorSortAdjFind(const char *b, size_t sequence_length) {
  const char *e = b + sequence_length;
  std::vector<char> s(b, e);
  std::ranges::sort(s);
  return s.cend() == std::adjacent_find(s.cbegin(), s.cend());
}

bool IsUniqueVectorUpperBound(const char *b, size_t sequence_length) {
  const char *e = b + sequence_length;
  std::vector<char> s(sequence_length);
  for (auto i = b; i < e; i++) {
    const auto j = std::ranges::upper_bound(s, *i); // first greater
    if (j > s.cbegin()) {
      // we have something to compare with
      if (*i == *(j - 1))
        return false;
    }
    s.insert(j, *i);
  }
  return true;
}

static void BM_Day06_IsUniqueSet(benchmark::State &state) {
  for (auto _ : state) {
    IsUniqueSet(test, sequence_length_2);
  }
}

static void BM_Day06_IsUniqueUnorderedSet(benchmark::State &state) {
  for (auto _ : state) {
    IsUniqueUnorderedSet(test, sequence_length_2);
  }
}

static void BM_Day06_IsUniqueAbslFlatHashSet(benchmark::State &state) {
  for (auto _ : state) {
    IsUniqueAbslFlatHashSet(test, sequence_length_2);
  }
}

static void BM_Day06_IsUniqueVectorSortAdjFind(benchmark::State &state) {
  for (auto _ : state) {
    IsUniqueVectorSortAdjFind(test, sequence_length_2);
  }
}

static void BM_Day06_IsUniqueVectorUpperBound(benchmark::State &state) {
  for (auto _ : state) {
    IsUniqueVectorUpperBound(test, sequence_length_2);
  }
}

// Register the functions as a benchmark
BENCHMARK(BM_Day06_IsUniqueSet);
BENCHMARK(BM_Day06_IsUniqueUnorderedSet);
BENCHMARK(BM_Day06_IsUniqueAbslFlatHashSet);
BENCHMARK(BM_Day06_IsUniqueVectorSortAdjFind);
BENCHMARK(BM_Day06_IsUniqueVectorUpperBound);
} // namespace day06

TEST(AoC22, Day06) {
  const auto data = ReadData("06")[0];
  const auto is_unique = [](std::string_view data, size_t sequence_length,
                            size_t start) {
    const auto b = data.cbegin() + start;
    return day06::IsUniqueVectorSortAdjFind(b, sequence_length);
  };
  const auto solution = [&is_unique](std::string_view data,
                                     size_t sequence_length) {
    const size_t l = data.length() - sequence_length + 1;
    for (size_t i = 0; i < l; i++) {
      if (is_unique(data, sequence_length, i))
        return std::optional<size_t>(i + sequence_length);
    }
    return std::optional<size_t>();
  };

  EXPECT_EQ(solution(day06::test, day06::sequence_length_1), 7);
  EXPECT_EQ(solution(data, day06::sequence_length_1), 1651); // part 1

  EXPECT_EQ(solution(day06::test, day06::sequence_length_2), 19);
  EXPECT_EQ(solution(data, day06::sequence_length_2), 3837); // part 2
}