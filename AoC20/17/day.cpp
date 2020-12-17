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

typedef tuple<int,int,int> Point;
typedef set<Point> Config; 

inline int & gx( Point & p ) { return get<0>(p); }
inline int & gy( Point & p ) { return get<1>(p); }
inline int & gz( Point & p ) { return get<2>(p); }
inline int gx( const Point & p ) { return get<0>(p); }
inline int gy( const Point & p ) { return get<1>(p); }
inline int gz( const Point & p ) { return get<2>(p); }

inline bool isOn( const Config & c, const Point & p ) { return c.find( p ) != c.end(); }

int countOn( const Config & c, const Point & p ) {
    int count {0};
    for ( int x = gx(p) - 1; x <= gx(p) + 1; x++ ) {
        for ( int y = gy(p) - 1; y <= gy(p) + 1; y++ ) {
            for ( int z = gz(p) - 1; z <= gz(p) + 1; z++ ) {
                const Point p1 {x, y, z};
                if ( p == p1 ) continue;
                count += isOn(c, p1);
            }
        }
    }
    assert(count <= 26);
    return count;
}

int main() {

    Config configOn;

    const bool isFirstAnswer = true;

    ifstream f("input");
    string line;
    for ( int y = 0; getline(f, line); y++ ) {
        for ( int x = line.length(); --x >= 0; ) {
            const char ch = line[x];
            if ( ch == '#' ) configOn.insert( { x, y, 0 } );
        }
    }

    for ( int cycle = 1; cycle <= 6; cycle++ ) {
        cout << cycle << " " << configOn.size() << endl;
        Point mi { *configOn.cbegin() };
        Point ma { mi };
        for ( const auto & p : configOn ) {
            gx(mi) = min( gx(mi), gx(p) );
            gy(mi) = min( gy(mi), gy(p) );
            gz(mi) = min( gz(mi), gz(p) );

            gx(ma) = max( gx(ma), gx(p) );
            gy(ma) = max( gy(ma), gy(p) );
            gz(ma) = max( gz(ma), gz(p) );
        }

        Config newConfig {};
        for ( int x = gx(mi) - 2; x <= gx(ma) + 2; x++ ) {
            for ( int y = gy(mi) - 2; y <= gy(ma) + 2; y++ ) {
                for ( int z = gz(mi) - 2; z <= gz(ma) + 2; z++ ) {
                    const Point p { x, y, z };
                    const int count = countOn( configOn, p );
                    bool on = isOn( configOn, p );
                    // if ( on ) cout << gx(p) << ',' << gy(p) << ',' << gz(p) << " -> " << count << endl;
                    if ( on ) {
                        on = count == 2 || count == 3;
                    } else {
                        on = count == 3;
                    }
                    if ( on ) newConfig.insert(p);
                }
            }
        }
        configOn = newConfig;
    }
    // 249 - too low
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? configOn.size() : 2 ) << endl;

    return 0;
}
