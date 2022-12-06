#include "common.h"
#include <set> // 11 ms
// #include <flat_set> // C++23 not supported yet
// #include <unordered_set> // 23-11 ms
// #include "absl/container/flat_hash_set.h" // takes 25 ms stable

TEST(AoC22, Day06) {
  const auto data = ReadData("06")[0];
  constexpr auto sequence_length_1 = 4;
  const auto is_unique = [](std::string_view data, size_t sequence_length,
                            size_t start) {
    const auto b = data.cbegin() + start;
    const auto e = b + sequence_length;

    // 7 ms
    std::vector<char> s(sequence_length);
    for ( auto i = b; i < e; i++ ) {
        const auto j = std::ranges::upper_bound(s, *i); // first greater
        if ( j > s.cbegin() ) {
            // we have something to compare with
            if ( *i == *(j - 1) ) return false;
        }
        s.insert(j, *i);
    }
    return true;

    /* std::vector<char> s(b,e);
    std::ranges::sort( s );
    return s.cend() == std::adjacent_find( s.cbegin(), s.cend() ); // 10-12 ms
    */
    // std::set<char> s(b, e); // 11 ms
    // std::unordered_set<char> s(b, e); // 12 ms
    // absl::flat_hash_set<char> s(b, e); 25 ms
    return s.size() == sequence_length;
  };
  const auto solution = [&is_unique](std::string_view data,
                                     size_t sequence_length) {
    const auto l = data.length() - sequence_length + 1;
    for (auto i = 0; i < l; i++) {
      if (is_unique(data, sequence_length, i))
        return std::optional<size_t>(i + sequence_length);
    }
    return std::optional<size_t>();
  };

  constexpr auto sequence_length_2 = 14;
  constexpr auto test = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";

  EXPECT_EQ(solution(test, sequence_length_1), 7);
  EXPECT_EQ(solution(data, sequence_length_1), 1651); // part 1

  EXPECT_EQ(solution(test, sequence_length_2), 19);
  EXPECT_EQ(solution(data, sequence_length_2), 3837); // part 2
}