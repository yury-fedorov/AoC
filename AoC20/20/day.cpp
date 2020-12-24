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
const vector<Direction> Directions { UP, RIGHT, DOWN, LEFT };

const int SIDE = 10;

typedef pair<int,int> Point;

inline int inverse( const int v, const int n ) { return n - v - 1; };
Point original ( const Point & p, const int n ) { return p; };
Point rotate90 ( const Point & p, const int n ) { return { inverse( p.second, n ), p.first }; }
Point rotate180( const Point & p, const int n ) { return { inverse( p.first,  n ), inverse( p.second, n ) }; }
Point rotate270( const Point & p, const int n ) { return { p.second,               inverse( p.first,  n ) }; }
Point horFlip  ( const Point & p, const int n ) { return { p.first,                inverse( p.second, n ) }; }
Point verFlip  ( const Point & p, const int n ) { return { inverse( p.first,  n ), p.second }; }

typedef Point (*Transformer)( const Point &, const int );
enum TransformationType { ORIGINAL, ROTATE_90, ROTATE_180, ROTATE_270, HOR_FLIP, VER_FLIP };
const vector<TransformationType> TxTypes = { ORIGINAL, ROTATE_90, ROTATE_180, ROTATE_270, HOR_FLIP, VER_FLIP };
const vector<Transformer> Transformers = { original, rotate90, rotate180, rotate270, horFlip, verFlip };


typedef vector<string> Tile;
typedef map<int, Tile> Tiles;
typedef string Border;
typedef vector<Border> Borders;
typedef map<int, Borders> TileBordersMap;
typedef pair<Border,Border> TwoBorders;
typedef map<Point,int> Image;
typedef map<Border,int> OutBorderTileMap;
typedef map<Direction,Border> DirBorders;  
typedef set<TransformationType> TxOptions;

Border reverse( const Border & b ) {
    Border result(b);
    reverse( result.begin(), result.end() );
    return result;
}

inline Direction opposite( Direction d ) { return (Direction)( ( d + 2 ) % 4 ); }

inline TwoBorders minmax( const Border & b ) { return minmax(b, reverse(b)); } 

inline Border normalize( const Border & b ) { return minmax(b).first; };

Border border( const Tile & tile, Direction d ) {
    switch (d) {
        case UP: return tile[0];
        case DOWN: return reverse( *tile.rbegin() );
        case RIGHT: 
        case LEFT: {
            Border border;
            for ( const auto & line : tile ) {
                border += ( d == RIGHT) ? *line.rbegin() : line[0];
            }
            return ( d == RIGHT ) ? border : reverse( border );
        }
        default: assert(false);
    }
}

Borders borders( const Tile & t ) { return { border(t,UP), border(t,RIGHT), border(t,DOWN), border(t,LEFT) }; }

Border border( const Tiles & tiles, int id, Direction d ) { return border( tiles.at(id), d ); }

optional< TwoBorders > connection( const Tiles & tiles, int tile1, int tile2 ) {
    const Tile & t1 = tiles.at(tile1);
    const Tile & t2 = tiles.at(tile2);
    const Borders && b1 = borders(t1);
    const Borders && b2 = borders(t2);
    for ( const Border & b1i : b1 ) {
        for ( const Border & b2i : b2 ) {
            if ( b1i == b2i || b1i == reverse(b2i) ) {
                return TwoBorders { b1i, b2i };
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

Tiles init() {
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
    return tiles;
}

// ideal is one result - this one is faster than connection
vector< pair<int,Border> > next( const Tiles & tiles, int tile, const vector<int> & scope, Border border ) {
    vector< pair<int,Border> > result;
    const auto && br = reverse(border);
    for ( const int tile2 : scope ) {
        if ( tile2 == tile ) continue;
        const auto & t2 = tiles.at(tile2);
        const Borders && b2 = borders(t2);
        for ( const Border & b2i : b2 ) {
            if ( br == b2i ) {
                result.emplace_back( tile2, b2i );
                break;
            }
        }
    }
    return result;
}

void initSide( Point p, int & value(Point & p), const int maxValue, const Tiles & tiles, const vector<int> & scope,
        Image & image ) {
    for ( ; value(p) <= maxValue; ) {
        const int t1 = image.at( p );
        Point p0(p);
        value(p0) -= 1;
        const int t0 = image.at( p0 );
        const auto & op = connection(tiles, t0, t1 );
        assert( op.has_value() );
        const auto [ b1, b2 ] = op.value();
        const auto d2 = where( tiles, t1, b2 );
        const auto d2o = opposite(d2); // good for going through side but not for corner
        const auto b2o = border( tiles, t1, d2o );
        cout << "Searching " << t1 << " " << b2o << endl;
        auto && close = next( tiles, t1, scope, b2o );
        if ( close.empty() ) close = next( tiles, t1, scope, reverse(b2o) );
        const int t2 = close.cbegin()->first;
        value(p) += 1;
        cout << t2 << " " << p.first << "-" << p.second << endl;
        image.emplace( p, t2 );
    }
}

int & fx( Point & p ) { return p.first; }
int & fy( Point & p ) { return p.second; }

vector<int> tilesClose( const Tiles & tiles, const int tileId, const vector<int> & scope ) {
    vector<int> result;
    const auto && corner0borders = borders( tiles.at(tileId) );
    for ( const auto & b : corner0borders ) {
        auto && close = next( tiles, tileId, scope, b );
        if ( close.empty() ) close = next( tiles, tileId, scope, reverse(b) );
        if ( close.size() == 1 ) {
            const int sideId = close.cbegin()->first;
            cout << sideId << " " << b << endl;
            result.push_back(sideId);
        } 
    }
    return result;
}

void print(const Tile & t ) {
    for ( const auto & l : t ) {
        cout << l << endl;
    }
}

Tile transform( const Tile & t, Transformer tx ) {
    Tile result( t );
    const auto n = t.size();
    for ( auto x = 0; x < n; x++ ) {
        for ( auto y = 0; y < n; y++ ) {
            auto && p = tx( {x, y}, n );
            result[x][y] = t[fx(p)][fy(p)];
        }
    }
    return result;
}

DirBorders txBorders ( const Tile & t, Transformer tx, const OutBorderTileMap & out ) {
    const Tile && t1 = transform(t, tx);
    const auto && bl = borders( t1 );
    assert( bl.size() == 4 );
    DirBorders result;
    for ( const auto d : Directions ) {
        const auto & b = bl[d];
        const auto norm = normalize( b );
        const auto i = out.find( norm );
        if ( i == out.end() ) result.emplace( d, b );
    }
    assert( result.size() >= 2 );
    return result;
}

TxOptions options( const Tile & t, const DirBorders & facts, const OutBorderTileMap & out ) {
    TxOptions result;
    for ( const auto tx : TxTypes ) {
        const auto ft = Transformers[tx];
        const auto && t1 = transform( t, ft );
        const auto && t1b = txBorders( t1, ft, out );
        if ( facts.size() != t1b.size() ) continue;
        bool isMatching = true;
        for ( const auto & [direction, border] : facts ) {
            auto i = t1b.find( direction );
            if ( i == t1b.end() ) { isMatching = false; break; };
            if ( border != i->second ) { isMatching = false; break; }
        }
        if ( isMatching ) result.insert( tx );
    }
    assert( !result.empty() );
    return result;
}

int main() {
    Tiles && tiles = init();
    cout << tiles.size() << endl;

    // normalized border to tile IDs
    map< Border, set<int> > borderIdsMap;
    for ( const auto [ id, tile ] : tiles ) {
        const Borders && bs = borders( tile );
        cout << id << endl;
        for ( const auto & b : bs ) {
            cout << b << " " << reverse(b) << endl;
            borderIdsMap[normalize(b)].insert( id );
        }
    }
    cout << borderIdsMap.size() << endl;

    // unique tile ID for border (sign of outer side)
    map<int, int> idCount;
    OutBorderTileMap outBorderTileId;
    for ( const auto [ b, ids ] : borderIdsMap ) {
        if ( ids.size() == 1 ) { 
            const int id = *ids.cbegin();
            cout << b << " " << id << endl;
            idCount[id] += 1;
            outBorderTileId.emplace( b, id );
        }
    }
    unsigned long long answer1 = 1;
    vector<int> corners; // 4 corners
    vector<int> sides; // all sides (tile IDs) with one border out
    vector<int> sidesAndCorners;
    for ( const auto [ id, count ] : idCount ) {
        if ( count == 1 ) { sides.push_back(id); }
        else if ( count == 2 ) {
            cout << "Corner found: " << id << endl;
            answer1 *= id;
            corners.push_back(id);
        } else assert(false);
        sidesAndCorners.push_back(id);
    }

    cout << "Answer 1: " << answer1 << endl;
    assert( answer1 == 8581320593371 );

    assert( idCount.size() == 44 ); // 4 corners and 40 borders
    assert( corners.size() == 4 );
    assert( sides.size() == 40 );

    Image image;
    const int corner0 = corners[0];
    image.emplace( Point{0,0}, corner0 );
    const auto && corner0Tiles = tilesClose( tiles, corner0, sides );
    for ( const int sideId : corner0Tiles ) {
        if ( image.size() == 1 ) image.emplace( Point{0,1}, sideId );
        else image.emplace( Point{1,0}, sideId );
    }

    initSide( Point {1,0}, fx, SIDE, tiles, sidesAndCorners, image );
    initSide( Point {0,1}, fy, SIDE, tiles, sidesAndCorners, image );
    // till now we defined the first corner and its neighbour sides
    {
        const int cornerX0 = image.at( Point{11,0} );
        const int side0X0 = image.at( Point{10,0} );
        const auto && cornerX0Tiles = tilesClose( tiles, cornerX0, sides );
        for ( const int sideId : cornerX0Tiles ) {
            cout << sideId << endl;
            if ( sideId != side0X0 ) image.emplace( Point{11,1}, sideId );
        }
        initSide( Point{11,1}, fy, SIDE, tiles, sidesAndCorners, image );
    }

    {
        const int corner0X = image.at( Point{0, 11} );
        const int side00X = image.at( Point{0, 10} );
        const auto && corner0XTiles = tilesClose( tiles, corner0X, sides );
        for ( const int sideId : corner0XTiles ) {
            cout << sideId << endl;
            if ( sideId != side00X ) image.emplace( Point{1, 11}, sideId );
        }
        initSide( Point{1,11}, fx, SIDE - 1, tiles, sidesAndCorners, image );
    }
    // till now all borders and corners are done
    vector<int> center;
    for ( const auto & [ id, tile ] : tiles ) {
        if ( find( sidesAndCorners.cbegin(), sidesAndCorners.cend(), id ) == sidesAndCorners.cend() ) {
            center.push_back(id);
        } 
    }
    assert( center.size() == 100 );

    const int t10 = image.at( Point{1,0} );
    const auto && t10center = tilesClose( tiles, t10, center );
    assert( t10center.size() == 1 );
    image.emplace( Point{1,1}, t10center[0] ); // first center tile placed

    initSide( Point{1,1}, fy, SIDE - 1, tiles, center, image ); // first center column placed
    for ( int i = 1; i <= 10; i++ ) { // now we may put all center rows
        initSide( Point{1,i}, fx, SIDE - 1, tiles, center, image );
    }

    // all image is ready now
    set<int> integrityCheck;
    for ( int y = 0; y <= 11; y++ ) {
        for ( int x = 0; x <= 11; x++ ) {
            const int id = image.at( Point{ x, y } );
            cout << id << '\t';
            integrityCheck.insert(id);
        }
        cout << endl;       
    }
    assert( integrityCheck.size() == 144 ); // all image is valid (every tile is used once)

    // TODO 
    // 1. we need to rotate every tile to align parts close each other
    //      a. we need algorithm for rotation
    //      b. we need algorithm for fliping
    //      c. we start from corners (sure orientation is easy to detect)
    //      d. we continue with sides (where out border is easy to detect)
    // 2. the borders of each tile are not part of the actual image; we need to remove them
    // 3. search for monsters
    // 4. How many # are not part of a sea monster?
    {
        const int t00id = image.at( Point{0,0} );
        const auto & t00 = tiles.at( t00id );
        print(t00);
        const int t01id = image.at( Point{0,1} );
        const Tile & t01 = tiles.at( t01id );
        cout << "T(0,1)" << endl;
        print(t01);
        const int t10id = image.at( Point{1,0} );
        const Tile & t10 = tiles.at( t10id );
        cout << "T(1,0)" << endl;
        print(t10);
        cout << endl;
        const auto o01 = connection(tiles, t00id, t01id ).value().first;
        const auto o10 = connection(tiles, t00id, t10id ).value().first;
        cout << o01 << endl;
        cout << o10 << endl;

        assert( outBorderTileId.size() == ( 4 * (SIDE + 2) ) );

        for ( const auto tx : TxTypes ) {
            cout << tx << endl;
            const auto f = Transformers[tx];
            const auto && tr = transform(t00, f );
            // print( tr );
            // cout << endl;
            const auto && bs = txBorders( tr, f, outBorderTileId );
            for ( const auto & [d, b] : bs ) {
                cout << "Dir -> " << d << " border -> " << b << endl;
            }
        }

        const auto && ts00 = options( t00, { { RIGHT, o10 }, { DOWN, o01 } }, outBorderTileId );



    }


    return 0;
}
