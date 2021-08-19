#include <iostream>
#include <assert.h>
#include <catch2/catch.hpp>

using namespace std;

typedef long long Int;
typedef pair<int,int> Position;

inline Int next( Int value ) { return ( value * 252533 ) % 33554393; }
inline Position next( const Position & p ) { 
    return ( p.second == 1 ) ? Position { 1,  p.first + 1 } : Position { p.first + 1, p.second -1 };
}

TEST_CASE( "Day25", "[25]" ) {
    const Position target { 3075, 2981 }; // row 2981, column 3075
    Int value { 20'151'125 };
    for ( Position position { 1, 1 }; position != target; position = next(position) ) {
        value = next(value);
        assert( value >= 0 );
    }
    REQUIRE( 9132360 == value );
}
