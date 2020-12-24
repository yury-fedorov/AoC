#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <assert.h>

using namespace std;

enum Color { White, Black };
enum Direction { East, SouthEast, SouthWest, West, NorthWest, NorthEast };
const vector<Direction> DirectionList { East, SouthEast, SouthWest, West, NorthWest, NorthEast };
const vector<string> Directions { "e", "se", "sw", "w", "nw", "ne" };

typedef vector<Direction> DirectionSeq;
typedef map< DirectionSeq, int > Map;
typedef pair<int,int> Position;
typedef map< Position, int > PosMap;
typedef unsigned long long Int;

void print( const DirectionSeq & s ) {
    for ( const auto d : s ) cout << Directions[d] << " ";
    cout << endl;
}

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

// on main lines X (first) are odd 
inline bool isMainLine( const Position & p ) { return ( abs( p.second ) % 2 ) == 0; }

Position totalShift( const DirectionSeq & s ) {
    Position result {0,0};
    for ( const auto d : s ) { 
        const auto sh = shift(d);
        result.first += sh.first;
        result.second += sh.second;
    }
    return result;
}

Direction opposite( Direction d ) { return (Direction) ( ( d + 3 ) % 6 ); }

DirectionSeq instruction( const string & line ) {
    DirectionSeq seq;
    optional<char> first {};
    for ( const char ch : line ) {
        if ( first.has_value() ) {
            const string sd = { first.value(), ch };
            assert( sd.length() == 2);
            const int sb = seq.size();
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

DirectionSeq normalize( DirectionSeq seq ) {
    map<Direction,int> counter;
    
    for ( const Direction d : seq ) {
        const Direction od = opposite(d);
        int & odv = counter[od];
        if ( odv > 0 ) odv -= 1;
        else counter[d] += 1;
    }
    
    const static DirectionSeq ZeroW = { NorthWest, SouthWest, East }; // nw sw e
    const static DirectionSeq ZeroE = { NorthEast, SouthEast, West }; // ne se w
    const static vector<DirectionSeq> ZeroList = { ZeroW, ZeroE };
    for ( const auto & zs : ZeroList ) {
        while ( true ) {
            bool isZeroSeqPresent = true;
            for ( const auto d : zs ) {
                isZeroSeqPresent &= ( counter[d] > 0 );
            }
            if ( !isZeroSeqPresent ) break;
            for ( const auto d : zs ) {
                counter[d] -= 1;
            }    
        }
    }

    DirectionSeq result; // reserve enough space
    for ( const auto [ d, c ] : counter ) {
        for ( int i = 0; i < c; i++ ) result.push_back( d );
    }
    sort( result.begin(), result.end() );
    // print( result );
    return result;
}

inline bool isBlack( int flipCount ) { return ( ( flipCount % 2 ) == 1 ); }

Int countBlack( const PosMap & counter ) {
    Int result = 0;
    for ( const auto & [sh, c] : counter ) result += isBlack( c );
    return result;
}

Int countBlackEver( const PosMap & counter ) {
    Int result = 0;
    for ( const auto & [sh, c] : counter ) result += c > 0;
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

int main() {
    const bool isTest = false;
    if ( isTest ) {
        const auto && dl = instruction( "esew" );
        print( dl );
        assert( dl.size() == 3 );
        const auto && dln = normalize(dl);
        assert( dln.size() == 1 );
        assert( dln[0] == SouthEast );
    }
    if ( isTest ) {
        const auto && dl1 = instruction( "nwwswee" );
        assert( dl1.size() > 2 );
        const auto && dln1 = normalize(dl1);
        assert( dln1.empty() );
    }

    const bool isFirstAnswer = true;

    vector<DirectionSeq> input;
    // Map counter;
    PosMap counter;

    ifstream f("input.txt");

    string line;
    while (getline(f, line)) {
        const auto && dl = instruction(line);
        input.push_back( dl );
        const auto && ndl = totalShift(dl);
        counter[ndl] += 1;
    }

    const auto answer1 = countBlack(counter);
    cout << "Answer 1: " << answer1 << endl;
    // assert( answer1 == 436 );

    typedef pair<Position,int> PosCount;
     cout << "After day 1: " << countBlackEver(counter) << endl;
    for ( int day = 2; day <= 10; day++ ) {
        const auto [minX, maxX] = minmax_element( counter.begin(), counter.end(), 
            []( const PosCount & a, const PosCount & b ) { return a.first.first < b.first.first; } );
        const auto [minY, maxY] = minmax_element( counter.begin(), counter.end(), 
            []( const PosCount & a, const PosCount & b ) { return a.first.second < b.first.second; } );

        const int B = 4;
        PosMap copy;
        for ( int y = minY->first.second - B; y <= maxY->first.second + B; y++ ) {
            const bool isMainLine = ( y % 2 ) == 0;
            for ( int x = minX->first.first - B; x <= maxX->first.first + B; x++ ) {
                const int dx = x % 2;
                if ( isMainLine ) {
                    if ( dx != 0 ) continue;
                } else {
                    if ( dx != 1 ) continue;
                }
                for ( const auto d : Directions ) {
                    const Position p { x, y }; 
                    const int black = countBlack(counter, p);
                    const int flipCount = counter[p];
                    if ( isBlack( flipCount ) ) {
                        copy[p] = flipCount + ( ( ( black == 0 ) || ( black > 2 ) ) ? 1 : 0 );
                    } else {
                        // white
                        copy[p] = flipCount + ( black == 2 );
                    }
                }
            }
        }
        const auto dayCounter = countBlackEver(copy);
        cout << "Day " << day << " " << dayCounter << endl;
        swap( counter, copy );
    }
    // 2063 -- too low
    const auto answer2 = countBlackEver(counter);
    cout << "Answer 2: " << answer2 << endl;

    return 0;
}
