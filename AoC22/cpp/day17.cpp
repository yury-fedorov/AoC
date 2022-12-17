// #include "absl/strings/numbers.h"
#include "common.h"
namespace day17 {
  using Long = long long;
  using Point = std::pair<int,int>;
  using Chamber = std::vector<std::string>;
  constexpr int k_chamber_width = 7;

  struct JetPattern {
    std::string_view pattern;
    size_t position;
    char next() noexcept {
      const char next = pattern[position];
      position = (position + 1) % pattern.length(); 
      return next;
    }
  };

  // TODO: do we need it or it could be data driven?
  enum class RockType { H_LINE, PLUS, L_SHAPE, V_LINE, SQUARE };
  
  constexpr std::array k_h_line = { Point{ 0, 0 }, Point{1,0}, Point{2,0}, Point{3,0} };
  // TODO other 4 rocks
  constexpr std::array k_rocks = { k_h_line }; // TODO rocks here

  void FallOnePiece( Chamber & chamber, JetPattern & jet ) noexcept {
  }

  size_t Answer1( std::string_view jet_pattern_file ) noexcept {
    const auto data = ReadData(jet_pattern_file).at(0);
    JetPattern jet { data, 0 };
    Chamber chamber;
    for ( int rock_id = 0; rock_id < 2022; rock_id++ ) {
      FallOnePiece(chamber, jet);
    }
    return chamber.size(); // assumption that chamber lines are only filled by figures
  }
}

TEST(AoC22, Day17) {
  const auto answer1 = day17::Answer1("17-sample");
  /*
  std::vector<long> sums;
  long sum{0};
  for (const std::string &line : data) {
    if (long value{0}; absl::SimpleAtoi(line, &value)) {
      sum += value;
    } else {
      sums.insert(std::ranges::upper_bound(sums, sum), sum);
      sum = 0;
    }
  }
  const auto sum_highest_n = [&sums](size_t n) {
    return std::accumulate(sums.rbegin(), sums.rbegin() + n, 0);
  };
  EXPECT_EQ(sum_highest_n(3), 206643);
  */
  EXPECT_EQ(answer1, 3068);
}
