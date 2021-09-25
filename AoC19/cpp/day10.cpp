#include "common.h"

using namespace std;

namespace day10 {

    using Point =  pair<int,int>;
    using Map = vector<string>;

    char at( const Map & map, int x, int y ) noexcept { return map[y][x]; }
    char at( const Map & map, const Point & p ) noexcept { return at(map, p.first, p.second); }

    // https://en.wikipedia.org/wiki/Prime_number
    constexpr array<int,10> primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

    auto common_denominator( int a, int b ) noexcept {
        const auto aa = abs(a);
        const auto ab = abs(b);
        const auto mi0 = min( aa, ab );
        auto mi = mi0;
        auto ma = max( aa, ab );
        if ( mi < 2 ) return ma;
        const int half_mi = mi >> 1;
        for ( const auto prime : primes
            | rv::filter( [half_mi](const auto p) { return p <= half_mi; } ) ) {
            while (true) {
                const auto mi_d = div( mi, prime );
                if ( mi_d.rem != 0 ) break;
                const auto ma_d = div( ma, prime );
                if ( ma_d.rem != 0 ) break;
                mi = mi_d.quot;
                ma = ma_d.quot;
            }
        }
        Ensures(mi0 > 0 &&  mi > 0);
        return ( mi0 == mi ) ? 1 : ( mi0 / mi );
    }

    // constexpr char asteroid = '#';

    using Point = pair<int,int>;

    auto answer1( const Map & /*map*/ ) {
        // TODO - normalization x,y pair
        // TODO - put normalized x,y pairs in set
        // TODO - size of set
        set<Point> passed;

        return (int)passed.size();
    }

    auto answer2( const Map & ) {
        return 0;
    }
}

TEST_CASE( "Day10", "[10]" ) {
    using namespace day10;

    Map map;
    ifstream f("../data/day10.txt");
    string line;
    while ( getline(f, line) ) {
        map.push_back(line);
    }

    SECTION( "10-1" ) {
        REQUIRE( answer1(map) == -1 );
    }

    SECTION( "10-2" ) {
        REQUIRE( answer2(map) == -2 );
    }
}
