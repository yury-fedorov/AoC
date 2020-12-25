#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <assert.h>

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
Point bothFlip ( const Point & p, const int n ) { return verFlip( horFlip( p, n ), n ); }

typedef Point (*Transformer)( const Point &, const int );
enum TransformationType { ORIGINAL, ROTATE_90, ROTATE_180, ROTATE_270, HOR_FLIP, VER_FLIP, BOTH_FLIP };
const vector<TransformationType> TxTypes = { ORIGINAL, ROTATE_90, ROTATE_180, ROTATE_270, HOR_FLIP, VER_FLIP, BOTH_FLIP };
const vector<Transformer> Transformers = { original, rotate90, rotate180, rotate270, horFlip, verFlip, bothFlip };

enum FactType { EventualReverse, ExactMatch };

typedef vector<string> Tile;
typedef map<int, Tile> Tiles;
typedef string Border;
typedef vector<Border> Borders;
typedef map<int, Borders> TileBordersMap;
typedef pair<Border,Border> TwoBorders;
typedef map<Point,int> Image;
typedef map<Border,int> OutBorderTileMap;
typedef map<Direction,Border> DirBorders;
typedef map<Direction, pair<Border,FactType> > Facts; // bool - exact or not
typedef set<Tile> TxOptions;
typedef map<Point,Tile> ImageTx;

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
            if ( normalize(b1i) == normalize(b2i) ) {
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
int fx( const Point & p ) { return p.first; }
int fy( const Point & p ) { return p.second; }

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

void print( const DirBorders & db ) { 
    for ( const auto & [d,b] : db ) cout << "Dir: " << d << " b: " << b << endl;
}

void print( const Facts & facts ) { 
    for ( const auto & [d,f] : facts ) cout << "Dir: " << d << " Border: " << f.first << " Exact: " << ( f.second == ExactMatch ) << endl;
}

char & at( Tile & t, const Point & p ) { return t[fy(p)][fx(p)]; }
char at( const Tile & t, const Point & p ) { return t[fy(p)][fx(p)]; }
Point fxy( int x, int y ) { return {x, y}; }

Tile transform( const Tile & t, Transformer tx ) {
    Tile result( t );
    const int n = t.size();
    for ( int y = 0; y < n; y++ ) {
        for ( int x = 0; x < n; x++ ) {
            const auto && p0 = fxy( x, y );
            const auto && p1 = tx( p0, n );
            at( result, p1 ) = at( t, p0 );
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

const Border OUT_BORDER { "__________" };

bool isMatching( const Facts & facts, const DirBorders & t1b ) {
    for ( const auto & [direction, f] : facts ) {
        const Border & border = f.first;
        if ( border == OUT_BORDER ) continue; // we do not match out borders
        auto i = t1b.find( direction );
        if ( i == t1b.end() ) { cerr << "No direction: " << direction << " Border: " << border << endl; return false; }
        const FactType & type = f.second;
        if ( type == ExactMatch ) {
            if ( border != i->second ) { 
                cerr << "Does not match exact: " << border << " vs " << i->second << endl; 
                return false; 
            }
        } else {
            if ( normalize( border ) != normalize( i->second ) ) { 
                cerr << "Does not match: " << border << " vs " << i->second << endl; 
                return false; 
            }
        }
    }
    return true;
}

TxOptions options( const Tile & t, const Facts & facts, const OutBorderTileMap & out ) {
    TxOptions result;
    cout << "Facts: " << endl;
    print(facts);

    for ( const auto tx : TxTypes ) {
        cout << "Tx: " << tx << endl;
        const auto ft = Transformers[tx];
        const auto && t1 = transform( t, ft );
        print(t1);
        const auto && t1b = txBorders( t1, original, out );
        print(t1b);
        if ( isMatching( facts, t1b ) ) { cout << "Match" << endl; result.insert( t1 ); }
    }
    return result;
}

const int LAST_LINE = SIDE + 1; // 11

set<Tile> detectTx( const Point & p, ImageTx & imageTx, const Tiles & tiles, const Image & image, const OutBorderTileMap & out ) {
    // up to 4 borders in around can be known
    Facts facts;
    // set empty borders
    const auto x = fx(p);
    const auto y = fy(p);
    const static auto outBorderFact = make_pair( OUT_BORDER, ExactMatch );
    if      ( x == 0         ) facts.emplace( LEFT,  outBorderFact );
    else if ( x == LAST_LINE ) facts.emplace( RIGHT, outBorderFact );
    if      ( y == 0         ) facts.emplace( UP,    outBorderFact );
    else if ( y == LAST_LINE ) facts.emplace( DOWN,  outBorderFact );

    set<Direction> ds { Directions.cbegin(), Directions.cend() };
    for ( const auto & [ d, b ] : facts ) ds.erase( d );

    const int tileId = image.at(p);
    for ( const auto d : ds ) {
        const auto dx = d == LEFT ? -1 : d == RIGHT ? 1 : 0;
        const auto dy = d == UP   ? -1 : d == DOWN  ? 1 : 0;
        const Point p1 { x + dx, y + dy };
        auto j = imageTx.find( p1 );
        if ( j != imageTx.end() ) {
            // found clean tile, where the border is exact
            const auto & clean = j->second;
            const auto && otherBorder = border( clean, opposite(d) );
            facts.emplace( d, make_pair( reverse(otherBorder), ExactMatch ) ); // on our side is always reverse
        } else {
            auto i = image.find( p1 );
            if ( i != image.end() ) {
                const auto o = connection(tiles, tileId, image.at(p1) );
                assert( o.has_value() );
                facts.emplace( d, make_pair( o.value().first, EventualReverse ) );
            } else assert( false );
        }
    }

    const auto & t = tiles.at(tileId);
    const auto && ts = options( t, facts, out );
    if ( ts.empty() ) {
        cout << endl;
        print(t);
        print( txBorders(t, original, out) );
        cout << "Facts: " << endl;
        print(facts);
        cerr << " No tx for: " << p.first << " " << p.second <<  endl;
        assert( false );
    }
    return ts;
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
        assert( outBorderTileId.size() == ( 4 * (SIDE + 2) ) );

        ImageTx imageTx;


        const Point p {0,2};
        const int tileId = image.at( p );
        cout << "Tile ID: " << tileId << endl;
        const auto & t = tiles.at( tileId );
        /*
        Tile t;
        t.push_back("123");
        t.push_back("456");
        t.push_back("789");
        */
        print( t );

        // for ( const auto tx : TxTypes ) 
        {
            // cout << tx << endl;
            // auto ft = Transformers.at( tx );
            auto ft = []( const Point & p, const int n ) { return verFlip( rotate180( p, n ), n ); };
            // auto ft = rotate180;
            cout << endl;
            print( transform( t, ft ) );
            const auto && tb = txBorders( t, ft, outBorderTileId );
            print(tb);
        }
         

/*        DirBorders facts = { { UP, "#..#.#...#" }, { RIGHT, "##....##.." }, { DOWN, ".###..##.." }, { LEFT, OUT_BORDER } };
        cout << isMatching( facts, tb );
        const auto && o = options( t, facts, outBorderTileId );
        assert( !o.empty() );
        for ( const auto & oi : o ) cout << oi << endl;
*/
        for ( auto x = 0; x <= LAST_LINE; x++ ) {
            for ( auto y = 0; y <= LAST_LINE; y++ ) {
                const Point p { x, y };
                const auto && txs = detectTx( p, imageTx, tiles, image, outBorderTileId );
                const auto & tx0 = *txs.cbegin();
                imageTx.emplace( p, tx0 );
                cout << endl;
                cout << "CLEAN TILE " << x << " " << y << " ID: " << image.at( p ) << endl;
                print( tx0 );
                cout << endl;
            }
        }
    }


    return 0;
}
