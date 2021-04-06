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
#include <catch2/catch.hpp>

using namespace std;

auto day06(const bool isFirstAnswer ) {
    auto sumUniqueAnswers = 0;
    set<char> uniqueInGroup;
    bool isNewSet = true;
    ifstream f("06/input.txt");
    string line;
    while (getline(f, line)) {
        if ( line.empty() ) {
            // group is over
            sumUniqueAnswers += uniqueInGroup.size();
            uniqueInGroup.clear();
            isNewSet = true; // for second question
        } else {
            set<char> one;
            const bool isOriginalSet = isFirstAnswer || isNewSet;
            set<char> & s = isOriginalSet ? uniqueInGroup : one;
            for ( char ch : line ) {
                s.insert(ch);
            }
            if (!isOriginalSet) {
                set <char> u;
                for ( const char ch : one ) {
                    if ( uniqueInGroup.find(ch) != uniqueInGroup.end() ) {
                        u.insert(ch);
                    }
                }
                swap( uniqueInGroup, u );
            }
            isNewSet = false;
        }
    }
    sumUniqueAnswers += uniqueInGroup.size();

    return sumUniqueAnswers;
}

TEST_CASE( "Day06", "[06]" ) {
    REQUIRE( 6911 == day06(true) );
    REQUIRE( 3473 == day06(false) );
}
