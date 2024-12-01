#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <set>
#include <tuple>

using namespace std;

typedef tuple<int, int, int, int> Point;
typedef set<Point> Config;

inline int &gx(Point &p) { return get<0>(p); }
inline int &gy(Point &p) { return get<1>(p); }
inline int &gz(Point &p) { return get<2>(p); }
inline int &gw(Point &p) { return get<3>(p); }
inline int gx(const Point &p) { return get<0>(p); }
inline int gy(const Point &p) { return get<1>(p); }
inline int gz(const Point &p) { return get<2>(p); }
inline int gw(const Point &p) { return get<3>(p); }

inline bool isOn(const Config &c, const Point &p) {
  return c.find(p) != c.end();
}

int countOn(const Config &c, const Point &p) {
  int count{0};
  for (int x = gx(p) - 1; x <= gx(p) + 1; x++) {
    for (int y = gy(p) - 1; y <= gy(p) + 1; y++) {
      for (int z = gz(p) - 1; z <= gz(p) + 1; z++) {
        for (int w = gw(p) - 1; w <= gw(p) + 1; w++) {
          const Point p1{x, y, z, w};
          if (p == p1)
            continue;
          count += isOn(c, p1);
        }
      }
    }
  }
  return count;
}

auto day17(const bool isFirstAnswer) {
  Config configOn;

  ifstream f("17/input");
  string line;
  for (int y = 0; getline(f, line); y++) {
    for (int x = line.length(); --x >= 0;) {
      const char ch = line[x];
      if (ch == '#')
        configOn.insert({x, y, 0, 0});
    }
  }

  for (int cycle = 1; cycle <= 6; cycle++) {
    // cout << cycle << " " << configOn.size() << endl;
    Point mi{*configOn.cbegin()};
    Point ma{mi};
    for (const auto &p : configOn) {
      gx(mi) = min(gx(mi), gx(p));
      gy(mi) = min(gy(mi), gy(p));
      gz(mi) = min(gz(mi), gz(p));
      gw(mi) = min(gw(mi), gw(p));

      gx(ma) = max(gx(ma), gx(p));
      gy(ma) = max(gy(ma), gy(p));
      gz(ma) = max(gz(ma), gz(p));
      gw(ma) = max(gw(ma), gw(p));
    }
    const int D = 1;
    const int wmi = isFirstAnswer ? 0 : gw(mi) - D;
    const int wma = isFirstAnswer ? 0 : gw(ma) + D;

    Config newConfig{};
    for (int x = gx(mi) - D; x <= gx(ma) + D; x++) {
      for (int y = gy(mi) - D; y <= gy(ma) + D; y++) {
        for (int z = gz(mi) - D; z <= gz(ma) + D; z++) {
          for (int w = wmi; w <= wma; w++) {
            const Point p{x, y, z, w};
            const int count = countOn(configOn, p);
            bool on = isOn(configOn, p);
            on = on ? (count == 2 || count == 3) : (count == 3);
            if (on)
              newConfig.insert(p);
          }
        }
      }
    }
    configOn = newConfig;
  }
  return configOn.size();
}

TEST_CASE("Day17", "[17]") {
  REQUIRE(286 == day17(true));
  REQUIRE(960 == day17(false));
}