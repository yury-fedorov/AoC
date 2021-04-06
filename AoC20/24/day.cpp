#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <assert.h>
#include <catch2/catch.hpp>

using namespace std;

enum Color { White, Black };
enum Direction { East, SouthEast, SouthWest, West, NorthWest, NorthEast };
const vector<Direction> DirectionList { East, SouthEast, SouthWest, West, NorthWest, NorthEast };
const vector<string> Directions { "e", "se", "sw", "w", "nw", "ne" };

typedef vector<Direction> DirectionSeq;
typedef pair<int,int> Position;
typedef map< Position, int > PosMap;

// to stick to integers, we remap values
const int SHIFT_ONE = 2;
const int SHIFT_HALF = 1;

Position shift( Direction d ) {
    switch ( d ) {
        case East: return {  SHIFT_ONE, 0 };
        case West: return { -SHIFT_ONE, 0 };
        case SouthEast: return {  SHIFT_HALF, -SHIFT_HALF };
        case SouthWest: return { -SHIFT_HALF, -SHIFT_HALF };
        case NorthWest: return { -SHIFT_HALF,  SHIFT_HALF };
        case NorthEast: return {  SHIFT_HALF,  SHIFT_HALF };
    }
}

inline Position shift( Direction d, const Position & p ) {
    const auto && sh = shift(d);
    return { p.first + sh.first, p.second + sh.second };
}

Position totalShift( const DirectionSeq & s ) {
    Position result {0,0};
    for ( const auto d : s ) { 
        const auto sh = shift(d);
        result.first += sh.first;
        result.second += sh.second;
    }
    return result;
}

DirectionSeq instruction( const string & line ) {
    DirectionSeq seq;
    optional<char> first {};
    for ( const char ch : line ) {
        if ( first.has_value() ) {
            const string sd = { first.value(), ch };
            assert( sd.length() == 2);
            const size_t sb = seq.size();
            for ( int i = Directions.size(); --i >= 0; ) {
                if ( Directions[i] == sd ) {
                    seq.push_back( (Direction)i );
                    break;
                }
            }
            first = {};
            assert( sb < seq.size() );
        } else {
            if ( ch == 'e' ) seq.push_back( East );
            else if ( ch == 'w' ) seq.push_back( West );
            else first = ch;
        }
    }
    return seq;
}

inline bool isBlack( int flipCount ) { return ( ( flipCount % 2 ) == 1 ); }

int countBlack( const PosMap & counter ) {
    int result = 0;
    for ( const auto & [sh, c] : counter ) result += isBlack( c );
    return result;
}

int countBlack( const PosMap & counter, const Position & p ) {
    int result = 0;
    for ( const auto d : DirectionList ) {
        const Position && p1 = shift( d, p );
        auto i = counter.find( p1 );
        if ( i == counter.end() ) continue; // it is white
        result += isBlack( i->second );
    }
    return result;
}

TEST_CASE( "Day24", "[24]" ) {
    PosMap counter;

    ifstream f("24/input.txt");
    string line;
    while (getline(f, line)) {
        const auto && dl = instruction(line);
        const auto && ndl = totalShift(dl);
        counter[ndl] += 1;
    }

    const auto answer1 = countBlack(counter);
    REQUIRE( answer1 == 436 );

    typedef pair<Position,int> PosCount;
    for ( int day = 1; day <= 100; day++ ) {
        const auto [minX, maxX] = minmax_element( counter.begin(), counter.end(), 
            []( const PosCount & a, const PosCount & b ) { return a.first.first < b.first.first; } );
        const auto [minY, maxY] = minmax_element( counter.begin(), counter.end(), 
            []( const PosCount & a, const PosCount & b ) { return a.first.second < b.first.second; } );

        const int B = 2;
        const int min_x = minX->first.first;
        const int max_x = maxX->first.first;
        const int min_y = minY->first.second;
        const int max_y = maxY->first.second;

        PosMap copy;
        for ( int y = (min_y - B); y <= (max_y + B); y++ ) {
            const bool isMainLine = ( abs(y) % 2 ) == 0;
            for ( int x = (min_x - B); x <= (max_x + B); x++ ) {
                const int dx = abs(x) % 2;
                if ( isMainLine ) {
                    if ( dx != 0 ) continue;
                } else {
                    if ( dx != 1 ) continue;
                }

                const Position p { x, y }; 
                const int black = countBlack(counter, p);
                const int flipCount = counter[p];
                auto result = isBlack( flipCount ) ? Black : White;
                if ( result == Black ) {
                    if ( ( black == 0 ) || ( black > 2 ) ) result = White;
                } else {
                    if ( ( black == 2 ) ) result = Black;
                }
                if ( result == Black ) copy[p] = Black;
            }
        }
        swap( counter, copy );
    }
    const auto answer2 = countBlack(counter);
    REQUIRE( answer2 == 4133 );
}