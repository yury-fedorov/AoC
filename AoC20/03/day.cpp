#include <iostream>
#include <vector>
#include <fstream>

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
     for ( auto y = 0; y < sizeY; y += dy, x += dx ) {
         treeCount += get(x, y) == '#';
     }
     return treeCount;
}

int main() {
    ifstream f("input.txt");
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

    cout << "Answer 1: " << c31 << endl;

    long long result = c11 * c31;
    result *= c51 * c71 * c12;
    cout << "Answer 2: " << result << endl; // too low

    return 0;
}
