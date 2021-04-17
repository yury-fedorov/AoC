#include <iostream>
#include <vector>
#include <fstream>
#include <catch2/catch.hpp>

using namespace std;

auto countTrees(const vector<string> & inputMap, int dx, int dy) {
    const auto get = [inputMap](int x, int y) { 
        const string & s = inputMap[y];
        const auto l = s.length();
        return s[x % l];
     };

     const auto sizeY = inputMap.size();
     auto x = 0;
     auto treeCount = 0;
     for ( size_t y = 0; y < sizeY; y += dy, x += dx ) {
         treeCount += get(x, y) == '#';
     }
     return treeCount;
}

TEST_CASE( "Day03", "[03]" ) {
    ifstream f("03/input.txt");
    vector<string> inputMap;
    string line;
    while (getline(f, line)) {
        inputMap.push_back(line);
    }

    const auto c11 = countTrees(inputMap, 1, 1);
    const auto c31 = countTrees(inputMap, 3, 1);
    const auto c51 = countTrees(inputMap, 5, 1);
    const auto c71 = countTrees(inputMap, 7, 1);
    const auto c12 = countTrees(inputMap, 1, 2);
    REQUIRE( c31 == 299 );

    long long result = c11 * c31;
    result *= c51 * c71 * c12;
    REQUIRE( result == 3621285278 );
}
