#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
#include <catch2/catch.hpp>

using namespace std;

inline long next( int subject, long value ) { return ( value * subject ) % 20201227; }

int loops( const int pk ) {
    const int subjectNumber = 7;
    long value = 1;
    for ( int loop = 0; true; loop++ ) {
        if ( value == pk ) return loop;
        value = next(subjectNumber, value);
    }
    return -1;
}

long encryptionKey( const int subjectNumber, const int loops ) {
    long value = 1;
    for ( int loop = 0; loop < loops; loop++ ) {
        value = next(subjectNumber, value);
    }
    return value;
}

TEST_CASE( "Day25", "[25]" ) {
    vector<int> input;

    ifstream f("25/input.txt");
    string line;
    while (getline(f, line)) {
        input.push_back( stoi(line) );
    }

    assert( loops( 5764801  ) == 8 );
    assert( loops( 17807724 ) == 11 );
    assert( encryptionKey(17807724, 8) == 14897079 );

    const int pk1 = input[0];
    const int l1 = loops( pk1 );
    const int pk2 = input[1];
    const int l2 = loops( pk2 );

    const auto ek12 = encryptionKey(pk1, l2);
    const auto ek21 = encryptionKey(pk2, l1);
    assert( ek12 == ek21 );
    REQUIRE( ek12 == 711945 );
}