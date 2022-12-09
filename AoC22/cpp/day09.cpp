#include "absl/strings/numbers.h"
#include "common.h"
#include "absl/container/flat_hash_set.h"
#include "absl/container/flat_hash_map.h"

TEST(AoC22, Day09) {
  const auto data = ReadData("09");
  using Point = std::pair<int,int>;
  using Set = absl::flat_hash_set<Point>;
  auto head = std::make_pair(0,0);
  auto tail = head;
  Set tail_path;

  constexpr int kKnots = 10;
  std::array<Point,kKnots> knots2 = { head, head, head, head, head, head, head, head, head, head };
  Set tail_path_2;
  tail_path_2.insert(tail);

  tail_path.insert(tail);
  const absl::flat_hash_map<char,Point> shift = { { 'U', {0,-1} }, { 'D', {0,1} }, { 'L', {-1,0} }, {'R', {1,0}} };
  for (const std::string &line : data) {
    if ( line.empty() ) continue;
    const std::vector<std::string_view> p = absl::StrSplit( line, ' ' );
    const char dir = p[0][0];
    int n {0};
    if ( !absl::SimpleAtoi(p[1], &n) ) FAIL();
    const auto [dx,dy] = shift.at(dir);
    auto make_move = []( Point & point, const int dx, const int dy ) {
        auto [x,y] = point;
        x += dx;
        y += dy;
        point = {x, y};
    };
    auto sign = []( auto x ) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); };
    for ( int i = 0; i < n; i++ ) {
        make_move( head, dx, dy );
        auto [xh, yh] = head;
        auto [xt, yt] = tail;
        const auto dx = xh - xt;
        const auto dy = yh - yt;
        if ( std::max( abs(dx), abs(dy) ) > 1 ) {
            // need to move tail, the head is not adjucent anymore
            make_move( tail, sign( dx ), sign(dy) );
            tail_path.insert(tail);
        }
    }
  }
  EXPECT_EQ(tail_path.size(), 5902);
  EXPECT_EQ(tail_path_2.size(), 0);
}