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

/* commented code is at least 2 times faster to solve the task and is more scalable
void init( const vector<int> & v, int b, CountMinMap & m ) {
    for ( ; b >= 0; b-- ) {
        const CountMin r = sum( v, b );
        const auto n = r.first;
        CountMin prev = m[n];
        if ( prev.first == 0 ) prev.second = r.second;
        m[n] = make_pair( prev.first + 1, min( prev.second, r.second ) );
    }
}

auto split( const vector<int> & containers, const int target ) {
    auto low = 0;
    auto it = containers.cbegin();
    for ( ; it != containers.cend(); it++ ) {
        const auto next = low + *it;
        if ( next >= target ) { 
            break;
        }
        low = next;
    }
    return make_pair( it, low );
}
*/

TEST_CASE( "Day17", "[17]" ) {
    vector<int> containers;
    
    ifstream f("17/input.txt");
    string line;
    while (getline(f, line)) {
        containers.push_back( stoi(line) );
    }
    const int TARGET = 150;
    /*
    sort( containers.begin(), containers.end() );

    const auto sp = split(containers, TARGET);
    const auto h = sp.first - containers.cbegin();
    const auto firstHigh = *sp.first;

    CountMinMap lowMap;
    vector<int> lowCo(containers.cbegin(), sp.first);
    init( lowCo, maxCounter(h), lowMap );

    CountMinMap highMap;
    const vector<int> highCo( sp.first, containers.end() );
    init( highCo, maxCounter( highCo.size() ), highMap );

    int minSum = INT_MAX;
    long long answer1 = 0;
    for ( auto cl : lowMap ) {
        const auto ch = highMap.find( TARGET - cl.first );
        if ( ch != highMap.end() ) {
            minSum = min(minSum, cl.second.second + ch->second.second);
            answer1 += ( cl.second.first * ch->second.first );
        }
    }
    */

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
