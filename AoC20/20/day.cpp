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

auto minmax( const Border & b ) { return minmax(b, reverse(b)); } 

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

int main() {
    const bool isFirstAnswer = false;

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

    TileBordersMap borderMap;
    set<Border> allBoarders;
    for ( const auto [ id, tile ] : tiles ) {
        const Borders && bs = borders( tile );
        borderMap.emplace( id, bs );
        cout << id << endl;
        for ( const auto & b : bs ) {
            /*
            allBoarders.emplace( b );
            allBoarders.emplace( reverse(b));
            */
           // allBoarders.emplace( minmax(b).first );
           cout << b << endl;
        }
    }
    cout << allBoarders.size() << endl;


    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
