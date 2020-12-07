#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <numeric>
#include <assert.h>
#include <climits>

using namespace std;

typedef long long Score;
typedef vector<int> Composition;

enum Property {
    Capacity, Durability, Flavor, Texture, Calories
};

int score( const Composition & properties, int qty, Property prop) {
    return qty * properties.at(prop);
}

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");

    string line;
    while (getline(f, line)) {
        /* regex draft
        regex re("(\\d+)-(\\d+) ([a-z]): ([a-z]+)");
        smatch what;
        if( regex_match( line, what, re )) {
            const auto & a = what[1];
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
        */
    }

    Score maxScore = -1;
    Composition best;
    /*
        Sprinkles:      capacity 2, durability 0, flavor -2,    texture 0,  calories 3
        Butterscotch:   capacity 0, durability 5, flavor -3,    texture 0,  calories 3
        Chocolate:      capacity 0, durability 0, flavor 5,     texture -1, calories 8
        Candy:          capacity 0, durability -1, flavor 0,    texture 5,  calories 8
    */

    vector<Composition> properties;
    properties.emplace_back(2, 0, -2, 0, 3);
    properties.emplace_back(0, 5, -3, 0, 3);
    properties.emplace_back(0, 0, 5, -1, 8);
    properties.emplace_back(0, -1, 0, 5, 8);

    for (int sp = 0; sp <= 100; sp++) {
        for (int bu = 0; bu + sp <= 100; bu++) {
            const auto sum2 = bu + sp;
            for (int cho = 0; cho + sum2 <= 100; cho++) {
                const auto sum3 = cho + sum2;
                for (int can = 0; can + sum3 <= 100; can++) {
                    /*
                    const auto capacity = 2 * sp;
                    const auto durability = max( (5 * bu) + (-can), 0 );
                    if (durability == 0) continue;
                    const auto flavor = max( (-2 * sp) + (-3 * bu) + (5 * cho), 0 );
                    if (flavor == 0) continue;
                    const auto texture = 8 * (cho + can);
                    */

                    auto capacity = scope( properties[0], Capacity );
                    *durability* flavor* texture

                    const Score score = capacity * durability * flavor * texture;
                    if (score > maxScore) {
                        best = { sp, bu, cho, can, 0 };
                        maxScore = score;
                    }
                }
            }
        }
    }
    cout << best[0] << " " << best[1] << " " << best[2] << " " << best[3] << endl;
    cout << "Answer 1: " << maxScore << endl; // 385715200 - too high

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
