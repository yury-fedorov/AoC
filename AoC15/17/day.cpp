#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <climits>
#include <catch2/catch.hpp>

using namespace std;

typedef pair<int,int> CountMin;
typedef map<int,CountMin> CountMinMap;

CountMin sum( const vector<int> & v, int b ) {
    auto result = 0;
    auto totalOn = 0;
    for ( auto i = 0; b; i++ ) {
        const bool on = b & 1;
        result += on ? v[i] : 0;
        totalOn += on;
        b >>= 1;
    }
    return { result, totalOn };
}

int maxCounter( int n ) {
    return ( 1 << n ) - 1; // sets all valid bits to 1
}

// deleted code is at least 2 times faster to solve the task and is more scalable but this solution runs in 0.3 seconds

TEST_CASE( "Day17", "[17]" ) {
    vector<int> containers;
    
    ifstream f("17/input.txt");
    string line;
    while (getline(f, line)) {
        containers.push_back( stoi(line) );
    }
    const int TARGET = 150;
    
    auto answer1 = 0;
    int minSum = INT_MAX;
    for ( auto c = maxCounter(containers.size()); c > 0; c-- ) {
        const auto p = sum(containers, c);
        if ( p.first == TARGET ) {
            answer1++;
            minSum = min(minSum, p.second);
        }
    }
    REQUIRE( 4372 == answer1 );

    auto answer2 = 0;
    for ( auto c = maxCounter(containers.size()); c > 0; c-- ) {
        const auto p = sum(containers, c);
        answer2 += ( p.first == TARGET && p.second == minSum );
    }
    REQUIRE( 4 == answer2 );
}
