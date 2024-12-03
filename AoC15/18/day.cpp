#include <cassert>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<string> Field;

inline bool get(const Field &f, int x, int y) {
  const int n = f.size();
  return (x >= 0 && y >= 0 && x < n && y < n) ? (f[y][x] == '#') : false;
}

inline void set(Field &f, int x, int y, bool isOn) {
  f[y][x] = (isOn ? '#' : '.');
}

int countOn(const Field &f, int x, int y) {
  auto count = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0)
        continue;
      const auto x1 = x + dx;
      const auto y1 = y + dy;
      count += get(f, x1, y1);
    }
  }
  return count;
}

void turnOnCorners(Field &f) {
  const int n = f.size() - 1;
  set(f, 0, 0, true);
  set(f, 0, n, true);
  set(f, n, 0, true);
  set(f, n, n, true);
}

void print(const Field &f) {
  for (auto l : f) {
    cout << l << endl;
  }
  cout << endl;
}

int day18(const bool isFirstAnswer) {
  Field field;

  ifstream f("18/input.txt");
  const int t = 100;

  string line;
  while (getline(f, line)) {
    field.push_back(line);
  }

  assert(get(field, 3, 0));
  assert(countOn(field, 3, 0) == 2);

  const int n = field.size();
  for (int i = 0; i < t; i++) {
    if (!isFirstAnswer) {
      turnOnCorners(field);
    }
    // print(field);
    Field newField(field);
    for (int x = 0; x < n; x++) {
      for (int y = 0; y < n; y++) {
        bool isOn = get(field, x, y);
        const auto count = countOn(field, x, y);
        if (isOn) {
          isOn = (count == 2) || (count == 3);
        } else {
          isOn = (count == 3);
        }
        set(newField, x, y, isOn);
      }
    }
    swap(field, newField);
  }
  if (!isFirstAnswer) {
    turnOnCorners(field);
  }
  // print(field);

  auto count = 0;
  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      count += get(field, x, y);
    }
  }
  return count;
}

TEST_CASE("Day18", "[18]") {
  REQUIRE(1061 == day18(true));
  REQUIRE(1006 == day18(false));
}