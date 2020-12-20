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

using namespace std;

enum Direction { UP, RIGHT, DOWN, LEFT }; // clockwise rotation 

typedef vector<string> Tile;
typedef map<int, Tile> Tiles;
typedef string Border;
typedef vector<Border> Borders;
typedef map<int, Borders> TileBordersMap;

Border reverse( const Border & b ) {
    Border result(b);
    reverse( result.begin(), result.end() );
    return result;
}

inline Direction opposite( Direction d ) { return (Direction)( ( d + 2 ) % 4 ); }

pair<Border,Border> minmax( const Border & b ) { return minmax(b, reverse(b)); } 

Borders borders( const Tile & tile ) {
    Borders result { tile[0], "", reverse( *tile.rbegin() ), "" };
    string right, left;
    for ( const auto & line : tile ) {
        right +=  *line.rbegin();
        left += line[0];
    }
    result[RIGHT] = right;
    result[LEFT] = reverse( left );
    return result;
}

optional< pair<Border,Border> > connection( const Tiles & tiles, int tile1, int tile2 ) {
    const Tile & t1 = tiles.at(tile1);
    const Tile & t2 = tiles.at(tile2);
    const Borders && b1 = borders(t1);
    const Borders && b2 = borders(t2);
    for ( const Border & b1i : b1 ) {
        for ( const Border & b2i : b2 ) {
            if ( b1i == reverse(b2i) ) {
                return make_pair(b1i, b2i);
            }
        }
    }
    return {};
}

Direction where( const Tiles & tiles, int id, const Border & b ) {
    const Tile & t = tiles.at(id);
    const Borders && bs = borders(t);
    const auto i = find( bs.cbegin(), bs.cend(), b );
    return (Direction)( i - bs.cbegin() );
}

inline Border border( const Tiles & tiles, int id, Direction d ) {
    return borders( tiles.at(id) ).at(d);
}

int main() {

    Tiles tiles;

    ifstream f("input.txt");
    regex re("^Tile (\\d+):$");
    string line;
    while (getline(f, line)) {        
        smatch what;
        if( regex_match( line, what, re )) {
            const int id = stoi( what[1] );
            assert( id > 0 && id >= 1000 );
            
            Tile tile;
            while ( getline(f, line) && !line.empty() ) {
                tile.push_back( line );
            }
            tiles.emplace( id, tile );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    cout << tiles.size() << endl;

    // TileBordersMap borderMap;
    map< Border, set<int> > borderIdsMap;
    for ( const auto [ id, tile ] : tiles ) {
        const Borders && bs = borders( tile );
        cout << id << endl;
        for ( const auto & b : bs ) {
            cout << b << " " << reverse(b) << endl;
            borderIdsMap[minmax(b).first].insert( id );
        }
    }
    cout << borderIdsMap.size() << endl;

    map<int, int> idCount;
    for ( const auto [ b, ids ] : borderIdsMap ) {
        if ( ids.size() == 1 ) { 
            const int id = *ids.cbegin();
            cout << b << " " << id << endl;
            idCount[id] += 1;
        }
    }
    unsigned long long answer1 = 1;
    vector<int> corners;
    vector<int> sides;
    for ( const auto [ id, count ] : idCount ) {
        if ( count == 1 ) { sides.push_back(id); }
        else if ( count == 2 ) {
            cout << "Corner found: " << id << endl;
            answer1 *= id;
            corners.push_back(id);
        } else assert(false);
    }

    cout << "Answer 1: " << answer1 << endl;
    assert( answer1 == 8581320593371 );

    assert( idCount.size() == 44 ); // 4 corners and 40 borders
    assert( corners.size() == 4 );
    assert( sides.size() == 40 );

    const int SIDE = 10; // assumption that it is a square

    const int firstCorner = corners[0];
    for ( const int sideId : sides ) {
        const auto & op = connection(tiles, firstCorner, sideId );
        if ( op.has_value() ) {
            const auto [ b1, b2 ] = op.value();
            const auto d2 = where( tiles, sideId, b2 );
            const auto d2o = opposite(d2);
            const auto b2o = border( tiles, sideId, d2o );
            cout << b1 << " " << firstCorner << " " << sideId << " " 
                << where( tiles, firstCorner, b1 ) << " " 
                << d2 << " " << d2o << " " << b2o << endl;
        }
    }

    return 0;
}
