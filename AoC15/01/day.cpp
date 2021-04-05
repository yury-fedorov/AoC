#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <catch2/catch.hpp>

using namespace std;

TEST_CASE( "Day01", "[01]" ) {
    ifstream f("01/input.txt");
    stringstream buffer;
    buffer << f.rdbuf();
    const auto s = buffer.str();
    // const string s = "()())";

    auto floor = 0;
    auto position = 1;
    bool isPositionFound = false;
    for ( char next : s ) {
        switch ( next ) {
            case '(': floor++; break;
            case ')': floor--; break;
            default: cerr << "Unexpected symbol: " << next << endl; 
        }
        if (!isPositionFound){
            isPositionFound = (floor < 0);
            position += ( isPositionFound ? 0 : 1 );
        } 
    }
    // Question 1 - Final floor:
    REQUIRE( floor == 138 );

    // Question 2 - First time in basement:
    REQUIRE( position == 1771 );
}

