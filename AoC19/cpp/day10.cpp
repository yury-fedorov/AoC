#include "common.h"

using namespace std;

namespace day10 {

    using Point =  pair<int,int>;
    using Map = vector<string>;
    using Positions = set<Point>;

    char at( const Map & map, int x, int y ) noexcept { return map[y][x]; }
    char at( const Map & map, const Point & p ) noexcept { return at(map, p.first, p.second); }

    // https://en.wikipedia.org/wiki/Prime_number
    constexpr array<int,10> primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

    int common_denominator( int a, int b ) noexcept {
        const auto aa = abs(a);
        const auto ab = abs(b);
        const auto mi0 = min( aa, ab );
        auto mi = mi0;
        auto ma = max( aa, ab );
        if ( mi == 0 ) return ma;
        if ( mi == 1 ) return 1;
        const int half_mi = mi >> 1; // divide by two
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

    constexpr char asteroid = '#';

    using Point = pair<int,int>;

    int count_seen( const Positions & asteroids, Point from ) noexcept {
        Positions passed;
        for ( const auto & p : asteroids ) {
            int dx = p.first - from.first;
            int dy = p.second - from.second;
            if ( dx == 0 && dy == 0 ) continue;
            const int cd = common_denominator( dx, dy );
            assert( dx <= cd );
            assert( dx % cd == 0 );
            assert( dy <= cd );
            assert( dy % cd == 0 );
            dx /= cd;
            dy /= cd;
            passed.insert( { dx, dy } );
        }
        return (int)passed.size();
    }

    int answer1( const Map & map ) {
        return map.size(); // TODO - fixme now signal
        const int y_size = map.size();
        const int x_size = map[0].size();
        Positions all_asteriods;
        for ( auto x : rv::iota(0, x_size) ) {
            for ( auto y : rv::iota(0, y_size) ) {
                const Point p {x,y};
                if ( at(map, p ) == asteroid ) { all_asteriods.insert(p); }
            }
        }
        const auto seen = all_asteriods
            | rv::transform([&]( const auto & a ) { return count_seen( all_asteriods, a ); } );
        return r::max( seen );
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
        REQUIRE( answer1(map) == -1 ); // 26 is not the right answer
    }

    SECTION( "10-2" ) {
        REQUIRE( answer2(map) == -2 );
    }
}
