#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include <regex>
#include <numeric>
#include <assert.h>
#include <climits>
#include <string_view>
#include <catch2/catch.hpp>

using namespace std;

int codeToNumber( const string_view code, const char one, const char zero ) {
    int number = 0;
    int position = code.length() - 1;
    for ( char chPos : code ) {
        if ( chPos == one ) {
            number |= ( 1 << position );
        } else {
            assert( chPos == zero );;
        }
        position--;
    }
    return number;
}

int makeSeatId( const string_view rowCode, const string_view columnCode ) {
    const auto row = codeToNumber(rowCode, 'B', 'F');
    assert ( row <= 127 && row >= 0 );
    const auto column = codeToNumber(columnCode, 'R', 'L');
    assert ( column <= 7 and column >= 0 );
    return ( ( row * 8 ) + column );
}

TEST_CASE( "Day05", "[05]" ) {
    // cout << codeToNumber("FBFBBFF", 'B', 'F') << endl;
    // cout << codeToNumber("RLR", 'R', 'L') << endl;

    ifstream f("05/input.txt");

    auto maxSeatId = INT_MIN;
    auto minSeatId = INT_MAX;
    set<int> seats;

    string line;
    regex re("([BF]{7})([RL]{3})");
    smatch what;
    while (getline(f, line)) {
        if( regex_match( line, what, re )) {
            const string rowCode = what[1];
            const string columnCode = what[2];
            const auto seatId = makeSeatId( rowCode, columnCode );
            // cout << "row = " << rowCode << " column = " << columnCode << " seat ID = " << seatId << endl; 
            maxSeatId = max(maxSeatId, seatId);
            minSeatId = min(minSeatId, seatId);
            seats.insert(seatId);
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    REQUIRE(998 == maxSeatId);

    for ( int id = minSeatId; id < maxSeatId; id++ ) {
        if ( seats.find( id ) == seats.end() ) {
            REQUIRE(676 == id);
            return;
        }
    }
    FAIL();
}
