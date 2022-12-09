#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "common.h"

namespace day09 {
using Point = std::pair<int, int>;
using Set = absl::flat_hash_set<Point>;

void MakeMove(Point &point, const int dx, const int dy) {
  auto [x, y] = point;
  x += dx;
  y += dy;
  point = {x, y};
}

int Sign(auto x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }

void MakeMove(const Point &head, Point &tail, Set &tail_path) {
  const auto [xh, yh] = head;
  const auto [xt, yt] = tail;
  const auto dx = xh - xt;
  const auto dy = yh - yt;
  if (std::max(abs(dx), abs(dy)) > 1) {
    // need to move tail, the head is not adjucent anymore
    day09::MakeMove(tail, day09::Sign(dx), day09::Sign(dy));
    tail_path.insert(tail);
  }
}
} // namespace day09

TEST(AoC22, Day09) {
  const auto data = ReadData("09");
  auto head = std::make_pair(0, 0);
  auto tail = head;
  day09::Set tail_path;
  constexpr int kKnots = 10;
  std::array<day09::Point, kKnots> knots2 = {head, head, head, head, head,
                                             head, head, head, head, head};
  day09::Set dummy_path;
  day09::Set tail_path_2;
  tail_path_2.insert(tail);

  tail_path.insert(tail);
  const absl::flat_hash_map<char, day09::Point> shift = {
      {'U', {0, -1}}, {'D', {0, 1}}, {'L', {-1, 0}}, {'R', {1, 0}}};
  for (const std::string &line : data) {
    if (line.empty())
      continue;
    const std::vector<std::string_view> p = absl::StrSplit(line, ' ');
    const char dir = p[0][0];
    int n{0};
    if (!absl::SimpleAtoi(p[1], &n))
      FAIL();
    const auto [dx, dy] = shift.at(dir);
    for (int i = 0; i < n; i++) {
      day09::MakeMove(head, dx, dy);
      day09::MakeMove(head, tail, tail_path);

      day09::MakeMove(knots2[0], dx, dy);
      for (int ki = 0; ki < kKnots - 1; ki++) {
        auto &head2 = knots2[ki];
        auto &tail2 = knots2[ki + 1];
        auto &path2 = (ki == kKnots - 2) ? tail_path_2 : dummy_path;
        day09::MakeMove(head2, tail2, path2);
      }
    }
  }
  EXPECT_EQ(tail_path.size(), 5902);
  EXPECT_EQ(tail_path_2.size(), 2445);
}