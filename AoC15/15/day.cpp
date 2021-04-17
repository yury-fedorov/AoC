#include <iostream>
#include <vector>
#include <fstream>
#include <catch2/catch.hpp>

using namespace std;

typedef int Score;
typedef vector<int> Composition;

enum Property {
    Capacity, Durability, Flavor, Texture, Calories
};

inline int score( const vector<Composition> & properties, const vector<int> & qty, Property prop ) {
    int result = 0;
    for (int i = qty.size(); i-- >0; ) {
        result += qty[i] * properties[i][prop];
    }
    return max( result, 0 );
}

auto day15(const bool isFirstAnswer) {
    Score maxScore = 0;

    vector<Composition> properties;
    properties.push_back({ 2, 0, -2, 0, 3 });
    properties.push_back({ 0, 5, -3, 0, 3 });
    properties.push_back({ 0, 0, 5, -1, 8 });
    properties.push_back({ 0, -1, 0, 5, 8 });

    vector<Property> propList { Capacity, Durability, Texture, Flavor };

    for (int q0 = 0; q0 <= 100; q0++) {
        for (int q1 = 0; q0 + q1 <= 100; q1++) {
            const auto sum2 = q0 + q1;
            for (int q2 = 0; q2 + sum2 <= 100; q2++) {
                const auto q3 = 100 - (q2 + sum2);
                const vector<int> q { q0, q1, q2, q3 };
                if (!isFirstAnswer) {
                    const auto calories = score(properties, q, Calories);
                    if (calories != 500) continue;
                }
                Score totalScore = 1;
                for (auto p : propList) {
                    totalScore *= score(properties, q, p);
                    if (!totalScore) break;
                }
                maxScore = max(maxScore, totalScore);
            }
        }
    }
    return maxScore;
}

TEST_CASE( "Day15", "[15]" ) {
    REQUIRE( 21367368 == day15(true) );
    REQUIRE( 1766400  == day15(false));
}
